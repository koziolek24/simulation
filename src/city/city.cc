#include "city/city.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "agents/agents.h"
namespace zpr {

std::vector<std::weak_ptr<zpr::Agent>> Node::getPeople()
{
    return this->presentPeople_;
}
void Node::addPeople(const std::shared_ptr<zpr::Agent>& newAgent)
{
    this->presentPeople_.push_back(std::move(newAgent));
}
void Node::removePeople(const std::shared_ptr<zpr::Agent>& agentToRemove)
{
    this->presentPeople_.erase(
        std::remove_if(this->presentPeople_.begin(), this->presentPeople_.end(),
                       [&agentToRemove](const std::weak_ptr<zpr::Agent>& wAgent) {
                           auto agent = wAgent.lock();
                           return agent == agentToRemove;
                       }),
        this->presentPeople_.end());
}
std::string Node::getName()
{
    return this->name_;
}

std::string Node::getNodeType()
{
    std::string type = "";
    switch (this->type_) {
        case 0:
            type = "Node";
            break;
        case 1:
            type = "Metro";
            break;
        case 2:
            type = "Home";
            break;
        case 3:
            type = "Workplace";
            break;
        case 4:
            type = "Entertainment";
            break;
    }
    return type;
}
unsigned int Node::getPeopleCount()
{
    return (unsigned int)(this->presentPeople_.size());
}
unsigned int Node::getHealthyCount()
{
    unsigned int healthyCount = 0;
    for (const auto& wPeople : this->presentPeople_) {
        auto people = wPeople.lock();
        if (people->getHealthStatus() == Healthy)
            healthyCount++;
    }
    return healthyCount;
}
unsigned int Node::getInfectedCount()
{
    unsigned int infectedCound = 0;
    for (const auto& wPeople : this->presentPeople_) {
        auto people = wPeople.lock();
        healthStatus healthStatus = people->getHealthStatus();
        if (healthStatus == Infected || healthStatus == Sick)
            infectedCound++;
    }
    return infectedCound;
}

std::vector<std::weak_ptr<zpr::Node>> Metro::getConnectedNodes()
{
    return this->connectedNodes_;
}
void Metro::addConnection(const std::shared_ptr<zpr::Node>& newNode)
{
    this->connectedNodes_.push_back(std::move(newNode));
}

void Place::addConnectingStation(const std::shared_ptr<zpr::Metro>& connectingStation)
{
    this->connectingStation_ = std::move(connectingStation);
}
std::weak_ptr<zpr::Metro> Place::getConnecingStation()
{
    return this->connectingStation_;
}

unsigned int Workplace::getOpeningHour()
{
    return this->openingHour_;
}
unsigned int Workplace::getClosingHour()
{
    return this->closingHour_;
}
void Home::addPeopleLivingHere(std::shared_ptr<zpr::Agent> newPeople)
{
    this->PeopleLivingHere_.push_back(std::move(newPeople));
}
std::vector<std::weak_ptr<zpr::Agent>> Home::getPeopleLivingHere()
{
    return this->PeopleLivingHere_;
}

std::vector<std::weak_ptr<zpr::Node>> Metro::getAllNeighbours()
{
    return getConnectedNodes();
}

std::vector<std::weak_ptr<zpr::Node>> Place::getAllNeighbours()
{
    std::vector<std::weak_ptr<zpr::Node>> out;
    out.push_back(this->getConnecingStation());
    return out;
}

void Workplace::setClosingHour(unsigned int newClosingHour)
{
    this->closingHour_ = newClosingHour;
}

void Workplace::setOpeningHour(unsigned int newOpeningHour)
{
    this->openingHour_= newOpeningHour;
}

void Node::setName(std::string newName)
{
    this->name_ = newName;
}

void Node::setID(unsigned int newID)
{
    this->id_ = newID;
}

unsigned int Node::getID()
{
    return this->id_;
}

bool Node::operator==(Node& node)
{
    return node.getID() == this->getID();
}
}  // namespace zpr
