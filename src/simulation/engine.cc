#include "simulation/engine.h"

#include <ctime>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <vector>

#include "agents/agents.h"
#include "agents/peoples.h"
#include "city/city.h"

namespace zpr {
void simulationEngine::addWorker(std::shared_ptr<zpr::Worker> newWorker)
{
    this->allWorkers_.push_back(std::move(newWorker));
}
void simulationEngine::addNode(std::shared_ptr<zpr::Node> newNode)
{
    this->allNodes_.push_back(std::move(newNode));
}
std::vector<std::shared_ptr<zpr::Worker>> simulationEngine::getAllWorkers()
{
    return this->allWorkers_;
}
std::vector<std::shared_ptr<zpr::Node>> simulationEngine::getAllNodes()
{
    return this->allNodes_;
}
std::shared_ptr<zpr::Worker> simulationEngine::getWorkerById(unsigned int id)
{
    if (this->allWorkers_.size() > id) {
        return this->allWorkers_[id];
    }
    return nullptr;
}
std::shared_ptr<zpr::Node> simulationEngine::getNodeById(unsigned int id)
{
    if (this->allNodes_.size() > id) {
        return this->allNodes_[id];
    }
    return nullptr;
}

std::shared_ptr<zpr::Node> simulationEngine::findNode(std::string nodeName)
{
    for (const auto& node : this->getAllNodes()) {
        if (node->getName() == nodeName) {
            return node;
        }
    }
    return nullptr;
}
void simulationEngine::connectMetroToMetro(std::string firstStation, std::string secondStation)
{
    std::shared_ptr<zpr::Metro> stationA =
        dynamic_pointer_cast<zpr::Metro>(this->findNode(firstStation));
    std::shared_ptr<zpr::Metro> stationB =
        dynamic_pointer_cast<zpr::Metro>(this->findNode(secondStation));
    if (stationA && stationB) {
        stationA->addConnection(stationB);
        stationB->addConnection(stationA);
    }
    else {
        std::cout << "Could not connect those stations, they are invalid." << std::endl;
    }
    return;
}
void simulationEngine::connectMetroToPlace(std::string stationName,
                                           std::shared_ptr<zpr::Place> place)
{
    std::shared_ptr<zpr::Metro> station =
        std::dynamic_pointer_cast<zpr::Metro>(this->findNode(stationName));
    if (station) {
        station->addConnection(place);
        place->addConnectingStation(station);
    }
    else {
        std::cout << "Could not connect those nodes, they are invalid." << std::endl;
    }
    return;
}

void simulationEngine::calculateCheapestPath(std::shared_ptr<zpr::Node> start)
{
    std::queue<std::weak_ptr<zpr::Node>> queue;
    unsigned int startID = start->getID();
    this->cheapestPath_[{startID, startID}] = start;
    queue.push(start);
    while (!queue.empty()) {
        std::weak_ptr<zpr::Node> currentNode = queue.front();
        queue.pop();
        std::vector<std::weak_ptr<zpr::Node>> neighbours = currentNode.lock()->getAllNeighbours();
        for (const auto& neighbour : neighbours) {
            unsigned int neighbourID = neighbour.lock()->getID();
            if (!this->cheapestPath_.contains({neighbourID, startID})) {
                this->cheapestPath_[{neighbourID, startID}] = currentNode.lock();
                queue.push(neighbour);
            }
        }
    }
    return;
}

void simulationEngine::calculateAllPaths()
{
    for (auto node : this->getAllNodes()) {
        this->calculateCheapestPath(node);
    }
    return;
}

void simulationEngine::moveWorkerToTarget(std::shared_ptr<zpr::Worker> worker,
                                          std::weak_ptr<zpr::Node> target)
{
    std::weak_ptr<zpr::Node> oldPosition = worker->getPosition();
    oldPosition.lock()->removePeople(worker);
    std::weak_ptr<zpr::Node> nextNode = this->getNextNode(oldPosition, target);
    worker->setPosition(nextNode);
    nextNode.lock()->addPeople(worker);
}

std::weak_ptr<zpr::Node> simulationEngine::getNextNode(std::weak_ptr<zpr::Node> source,
                                                       std::weak_ptr<zpr::Node> target)
{
    unsigned int sourceID = source.lock()->getID();
    unsigned int targetID = target.lock()->getID();
    return this->cheapestPath_[{sourceID, targetID}];
}

std::weak_ptr<zpr::Node> simulationEngine::findNearestEntertainment(
    std::weak_ptr<zpr::Node> position)
{
    std::queue<std::weak_ptr<zpr::Node>> queue;
    std::set<unsigned int> visitedNodes;
    queue.push(position);
    visitedNodes.insert(position.lock()->getID());
    std::weak_ptr<zpr::Node> nearestEntertainment;
    while (!queue.empty()) {
        std::weak_ptr<zpr::Node> currentNode = queue.front();
        queue.pop();
        if (currentNode.lock()->getNodeType() == "Entertainment") {
            nearestEntertainment = currentNode;
            break;
        }
        for (auto neighbour : currentNode.lock()->getAllNeighbours()) {
            if (visitedNodes.find(neighbour.lock()->getID()) == visitedNodes.end()) {
                visitedNodes.insert(neighbour.lock()->getID());
                queue.push(neighbour);
            }
        }
    }
    return nearestEntertainment;
}

void simulationEngine::moveWorkerHome(std::shared_ptr<zpr::Worker> worker)
{
    std::weak_ptr<zpr::Node> position = worker->getPosition();
    std::weak_ptr<zpr::Node> target = worker->getHome();
    while (target.lock() != position.lock()) {
        std::weak_ptr<zpr::Node> nextNode = this->getNextNode(position, target);
        worker->setPosition(nextNode);
        position = worker->getPosition();
    }
    return;
}

void simulationEngine::moveWorkerWorkplace(std::shared_ptr<zpr::Worker> worker)
{
    std::cout << "Start ";
    std::weak_ptr<zpr::Node> position = worker->getPosition();
    std::cout << position.lock()->getName() << " ";
    std::weak_ptr<zpr::Node> target = worker->getWorkplace();
    std::cout << target.lock()->getName() << std::endl;
    while (target.lock() != position.lock()) {
        std::weak_ptr<zpr::Node> nextNode = this->getNextNode(position, target);
        this->moveWorkerToTarget(worker, nextNode);
        position = worker->getPosition();
    }
    return;
}

void simulationEngine::incrementTime()
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

unsigned int simulationEngine::getHour()
{
    return this->hour_;
}

unsigned int simulationEngine::getDay()
{
    return this->day_;
}

unsigned int simulationEngine::getMinute()
{
    return this->minute_;
}

void simulationEngine::doAction()
{
    unsigned int hour = this->getHour();
    unsigned int minute = this->getMinute();
    for (auto worker : this->getAllWorkers()) {
        if (hour == 7 and minute == 0) {
            worker->setTarget(WorkplaceTarget);
        }
        else if (hour == 16 and minute == 0) {
            int randomNumber = rand() % 10;
            if (randomNumber < 4)
                worker->setTarget(EntertainmentTarget);
            else{
                worker->setTarget(HomeTarget);
            }
        }else if(hour > 20 and minute%30 == 0)
        {
            int randomNumber = rand() % 10;
            if (randomNumber < 5)
                worker->setTarget(HomeTarget);
        }else if(hour == 24)
        {
            worker->setTarget(HomeTarget);
        }
    }
    for (auto worker : this->getAllWorkers()) {
        this->doWorkerAction(worker);
    }
    this->incrementTime();
}

void simulationEngine::doWorkerAction(std::shared_ptr<zpr::Worker> worker)
{
    std::weak_ptr<zpr::Node> target;
    switch (worker->getTarget()) {
        case HomeTarget:
            target = worker->getHome();
            break;
        case WorkplaceTarget:
            target = worker->getWorkplace();
            break;
        case EntertainmentTarget:
            target = this->findNearestEntertainment(worker->getPosition());
            break;
    }
    if (worker->getPosition().lock() != target.lock()) {
        this->moveWorkerToTarget(worker, target);
    }
    return;
}

}  // namespace zpr
