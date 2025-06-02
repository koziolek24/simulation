#include "simulation/engine.h"

#include <agents/agents.h>
#include <city/city.h>

#include <cstdlib>
#include <ctime>
#include <logging.hpp>
#include <memory>
#include <queue>
#include <set>
#include <stdexcept>
#include <thread>
#include <vector>

#include "disease/disease.h"

namespace zpr {
void GraphManager::addWorker(std::shared_ptr<zpr::Worker> newWorker)
{
    this->allWorkers_.push_back(newWorker);
}
void GraphManager::addNode(std::shared_ptr<zpr::Node> newNode)
{
    this->allNodes_.push_back(newNode);
    //
    std::string nodeName = newNode->getName();
    if (this->nodesByName_.contains(nodeName))
        LOG_WARNING "There are several nodes with name: ", nodeName);
    else
        this->nodesByName_[nodeName] = newNode;
}
const std::vector<std::shared_ptr<zpr::Worker>> GraphManager::getAllWorkers()
{
    return this->allWorkers_;
}
const std::vector<std::shared_ptr<zpr::Node>> GraphManager::getAllNodes()
{
    return this->allNodes_;
}
const std::shared_ptr<zpr::Worker> GraphManager::getWorkerById(unsigned int id)
{
    if (this->allWorkers_.size() > id) {
        return this->allWorkers_[id];
    }
    return nullptr;
}
const std::shared_ptr<zpr::Node> GraphManager::getNodeById(unsigned int id)
{
    if (this->allNodes_.size() > id) {
        return this->allNodes_[id];
    }
    return nullptr;
}

const std::shared_ptr<zpr::Node> GraphManager::getNodeByName(std::string nodeName)
{
    if (this->nodesByName_.contains(nodeName))
        return this->nodesByName_[nodeName];
    return nullptr;
}
void GraphCreator::connectMetroToMetro(std::string firstStation, std::string secondStation)
{
    std::shared_ptr<zpr::Metro> stationA =
        dynamic_pointer_cast<zpr::Metro>(this->getNodeByName(firstStation));
    std::shared_ptr<zpr::Metro> stationB =
        dynamic_pointer_cast<zpr::Metro>(this->getNodeByName(secondStation));
    if (stationA && stationB) {
        stationA->addConnection(stationB);
        stationB->addConnection(stationA);
    }
    else {
        LOG_WARNING "connectMetroToMetro: could not connect those nodes: ", firstStation, ":"
                    , secondStation);
    }
    return;
}
void GraphCreator::connectMetroToPlace(std::string stationName, std::shared_ptr<zpr::Place> place)
{
    std::shared_ptr<zpr::Metro> station =
        std::dynamic_pointer_cast<zpr::Metro>(this->getNodeByName(stationName));
    if (station) {
        station->addConnection(place);
        place->addConnectingStation(station);
    }
    else {
        LOG_WARNING  "connectMetroToPlace: Could not connect those nodes, they are invalid."
                    , stationName , place->getName());
    }
    return;
}

void GraphFinder::calculateCheapestPath(std::shared_ptr<zpr::Node> target)
{
    std::queue<std::weak_ptr<zpr::Node>> queue;
    unsigned int targetID = target->getID();
    this->cheapestPath_[{targetID, targetID}] = target;
    queue.push(target);
    while (!queue.empty()) {
        auto currentNode = queue.front().lock();
        queue.pop();
        if (!currentNode) {
            LOG_ERROR "calculateCheapestPath: currentNode is invalid");
            continue;
        }
        std::vector<std::weak_ptr<zpr::Node>> neighbours = currentNode->getAllNeighbours();
        for (const auto& neighbour : neighbours) {
            auto sNeighbour = neighbour.lock();
            if (!sNeighbour) {
                LOG_ERROR "calculateCheapestPath: sNeighbour is invalid");
                continue;
            }
            unsigned int neighbourID = sNeighbour->getID();
            if (!this->cheapestPath_.contains({neighbourID, targetID})) {
                this->cheapestPath_[{neighbourID, targetID}] = currentNode;
                queue.push(neighbour);
            }
        }
    }
    return;
}

void GraphFinder::calculateAllPaths(std::vector<std::shared_ptr<zpr::Node>> allNodes)
{
    for (auto node : allNodes) {
        this->calculateCheapestPath(node);
    }
    return;
}

void GraphTravel::moveWorkerToTarget(std::shared_ptr<zpr::Worker> worker,
                                     std::shared_ptr<zpr::Node> target)
{
    auto oldPosition = worker->getPosition().lock();
    if (!oldPosition) {
        LOG_ERROR  "moveWorkerToTarget: oldPosition is invalid - settingPosition to Home");
        auto newPosition = worker->getHome().lock();
        if (!newPosition) {
            throw std::runtime_error(
                "moveWorkerToTarget: Couldn't access worker's home - terminal error");
        }
        else {
            worker->setPosition(newPosition);
            newPosition->addPeople(worker);
            LOG_INFO "moveWorkerToTarget: succesfully set worker's position to his home");
            oldPosition = newPosition;
        }
    }
    oldPosition->removePeople(worker);
    std::shared_ptr<zpr::Node> nextNode = this->getNextNode(oldPosition, target);
    worker->setPosition(nextNode);
    nextNode->addPeople(worker);
    return;
}

const std::shared_ptr<zpr::Node> GraphFinder::getNextNode(std::shared_ptr<zpr::Node> source,
                                                          std::shared_ptr<zpr::Node> target)
{
    unsigned int sourceID = source->getID();
    unsigned int targetID = target->getID();
    return this->cheapestPath_[{sourceID, targetID}];
}

const std::shared_ptr<zpr::Node> GraphFinder::findNearestEntertainment(
    std::shared_ptr<zpr::Node> position)
{
    std::queue<std::weak_ptr<zpr::Node>> queue;
    std::set<unsigned int> visitedNodes;
    queue.push(position);
    visitedNodes.insert(position->getID());
    std::weak_ptr<zpr::Node> nearestEntertainment;
    while (!queue.empty()) {
        auto currentNode = queue.front().lock();
        queue.pop();
        if (!currentNode) {
            LOG_ERROR  "findNearestEntertainment: currentNode is invalid - skipping this node");
            continue;
        }
        if (currentNode->getNodeType() == entertainmentType) {
            nearestEntertainment = currentNode;
            break;
        }
        for (auto neighbour : currentNode->getAllNeighbours()) {
            auto sNeighbour = neighbour.lock();
            if (!sNeighbour) {
                LOG_ERROR "findNearestEntertainment: sNeighbour is invalid");
                continue;
            }
            unsigned int neigbourID = sNeighbour->getID();
            if (visitedNodes.find(neigbourID) == visitedNodes.end()) {
                visitedNodes.insert(neigbourID);
                queue.push(sNeighbour);
            }
        }
    }
    auto sEntertainment = nearestEntertainment.lock();
    if (!sEntertainment) {
        throw std::runtime_error(
            "findNearestEntertainment: sEntertainment is invalid - terminal error");
    }
    return sEntertainment;
}

void TimeManager::incrementTime()
{
    this->minute_ += 5;
    if (this->minute_ >= 60) {
        this->minute_ -= 60;
        ++this->hour_;
    }
    if (this->hour_ == 24) {
        this->hour_ = 0;
        ++this->day_;
    }
}

unsigned int TimeManager::getHour()
{
    return this->hour_;
}

unsigned int TimeManager::getDay()
{
    return this->day_;
}

unsigned int TimeManager::getMinute()
{
    return this->minute_;
}

void SimulationEngine::doAction()
{
    unsigned int hour = this->getHour();
    unsigned int minute = this->getMinute();
    unsigned int workStart = this->getWorkStartHour();
    unsigned int entertainmentStart = this->getEntertainmentStartHour();
    unsigned int entertainmentStartToLeave = this->getEntertainmentStartToLeaveHour();
    unsigned int entertainmentEnd = this->getEntertainmentEndHour();
    // double workSeverity = this->getWorkSeverity();
    // double entertainmentSeverity = this->getEntertainmentSeverity();
    double entertainmentGoProbability = this->getEntertainmentGoProbability();
    double entertainmentLeaveProbability = this->getEntertainmentLeaveProbability();
    zpr::target newTarget;
    for (auto worker : this->getAllWorkers()) {
        // double workerSeverity = worker->getVirus()->getSeverity();
        newTarget = worker->getTarget();
        if (hour == entertainmentEnd) {
            worker->setTarget(HomeTarget);
            continue;
        }
        if (hour == workStart) {
            newTarget = WorkplaceTarget;
        }
        else if (hour == entertainmentStart && minute == 0) {
            double randomNumber = ((double)rand()) / RAND_MAX;
            if (randomNumber <= entertainmentGoProbability)
                newTarget = EntertainmentTarget;
            else {
                newTarget = HomeTarget;
            }
        }
        else if (hour >= entertainmentStartToLeave && (minute == 0 || minute == 30)) {
            double randomNumber = ((double)rand()) / RAND_MAX;
            if (randomNumber <= entertainmentLeaveProbability)
                newTarget = HomeTarget;
        }
        if (newTarget != worker->getTarget()) {
            worker->setTarget(newTarget);
        }
    }

    std::vector<std::thread> threads;
    for (auto& worker : this->getAllWorkers()) {
        threads.emplace_back([this, worker]() { this->doWorkerAction(worker); });
    }

    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }

    this->incrementTime();
}

void SimulationEngine::doWorkerAction(std::shared_ptr<zpr::Worker> worker)
{
    std::shared_ptr<zpr::Node> target = nullptr;
    auto workerPosition = worker->getPosition().lock();
    if (!workerPosition) {
        LOG_ERROR "doWorkerAction: Worker position is invalid - trying to setPosition to Home");
        auto newPosition = worker->getHome().lock();
        if (!newPosition) {
            throw std::runtime_error(
                "doWorkerAction: Couldn't access worker's home - terminal error");
        }
        else {
            worker->setPosition(newPosition);
            newPosition->addPeople(worker);
            LOG_INFO "doWorkerAction: succesfully set worker's position to his home");
        }
    }

    switch (worker->getTarget()) {
        case HomeTarget:
            if (!worker->getHome().lock()) {
                throw std::runtime_error(
                    "doWorkerAction: Couldn't access worker's home - terminal error");
            }
            target = worker->getHome().lock();
            break;
        case WorkplaceTarget:
            if (!worker->getWorkplace().lock()) {
                throw std::runtime_error(
                    "doWorkerAction: Couldn't access worker's workplace - terminal error");
                break;
            }
            target = worker->getWorkplace().lock();
            break;
        case EntertainmentTarget:
            target = this->findNearestEntertainment(workerPosition);
            break;
    }

    if (workerPosition == target)
        return;

    auto nextNode = this->getNextNode(workerPosition, target);

    {
        std::scoped_lock lock(workerPosition->getMutex());
        workerPosition->removePeople(worker);
    }
    std::scoped_lock lock(nextNode->getMutex());
    nextNode->addPeople(worker);
    worker->setPosition(nextNode);

    for (auto& weakOther : nextNode->getPeople()) {
        auto other = weakOther.lock();
        if (!other)
            continue;
        if (other == worker)
            continue;
        unsigned int id = other->getVirus()->getSymptomsID();
        double contagiousness = this->getContagiousnessByID(id);
        if (contagiousness != 0)
            std::cout << contagiousness << ":" << other->getID() << std::endl;
        if (worker->getHealthStatus() == Healthy &&
            (other->getHealthStatus() == Infected || other->getHealthStatus() == Sick)) {
            if (contagiousness >= ((double)rand()) / RAND_MAX) {
                worker->setHealthStatus(Infected);
                worker->getVirus()->addSymptom(this->getInitialSymptom());
            }
        }
    }
    worker->getVirus()->evolve(this->getSymptoms());
    // if (worker->getHealthStatus() == Infected && rand() % 1000 < 1){
    //     worker->setHealthStatus(Healthy);
    // }

    return;
}

void SimulationEngine::setSeverityEntertainment(double severityEntertainment)
{
    this->severityEntertainment_ = severityEntertainment;
}
void SimulationEngine::setSeverityWork(double severityWork)
{
    this->severityWork_ = severityWork;
}
void SimulationEngine::setWorkStartHour(unsigned int hour)
{
    this->workStartHour_ = hour;
}
void SimulationEngine::setEntertainmentStartHour(unsigned int hour)
{
    this->entertainmentStartHour_ = hour;
}
void SimulationEngine::setEntertainmentEndHour(unsigned int hour)
{
    this->entertainmentEndHour_ = hour;
}
void SimulationEngine::setEntertainmentStartToLeaveHour(unsigned int hour)
{
    this->entertainmentStartToLeaveHour_ = hour;
}
void SimulationEngine::setEntertainmentGoProbability(double probability)
{
    this->entertainmentGoProbability_ = probability;
}
void SimulationEngine::setEntertainmentLeaveProbability(double probability)
{
    this->entertainmentLeaveProbability_ = probability;
}
unsigned int SimulationEngine::getWorkStartHour()
{
    return this->workStartHour_;
}
unsigned int SimulationEngine::getEntertainmentStartHour()
{
    return this->entertainmentStartHour_;
}
unsigned int SimulationEngine::getEntertainmentEndHour()
{
    return this->entertainmentEndHour_;
}
unsigned int SimulationEngine::getEntertainmentStartToLeaveHour()
{
    return this->entertainmentStartToLeaveHour_;
}
double SimulationEngine::getWorkSeverity()
{
    return this->severityWork_;
}
double SimulationEngine::getEntertainmentSeverity()
{
    return this->severityEntertainment_;
}
double SimulationEngine::getEntertainmentGoProbability()
{
    return this->entertainmentGoProbability_;
}
double SimulationEngine::getEntertainmentLeaveProbability()
{
    return this->entertainmentLeaveProbability_;
}

void SimulationEngine::setVirusManager(VirusManager manager)
{
    this->virusManager_ = manager;
}

double SimulationEngine::getContagiousnessByID(unsigned int id)
{
    return this->virusManager_.getContagiousnessByID(id);
}
double SimulationEngine::getMortalityByID(unsigned int id)
{
    return this->virusManager_.getMortalityByID(id);
}

std::vector<std::shared_ptr<zpr::Symptom>> SimulationEngine::getSymptoms()
{
    return this->virusManager_.getSymptoms();
}

VirusManager SimulationEngine::getVirusManager()
{
    return this->virusManager_;
}

std::shared_ptr<Symptom> SimulationEngine::getInitialSymptom()
{
    return this->getVirusManager().getInitialSymptom();
}

}  // namespace zpr
