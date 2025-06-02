#include "city/city.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

#include "agents/agents.h"
#include "visitors/NodeVisitor.h"
namespace zpr {

const std::vector<std::weak_ptr<zpr::Agent>> Node::getPeople()
{
    return this->presentPeople_;
}
void Node::addPeople(const std::shared_ptr<zpr::Agent>& newAgent)
{
    this->presentPeople_.push_back(newAgent);
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

enum nodeType Node::getNodeType()
{
    return this->type_;
}
unsigned int Node::getPeopleCount()
{
    std::scoped_lock lock(this->nodeMutex_);
    return (unsigned int)(this->presentPeople_.size());
}
unsigned int Node::getHealthyCount()
{
    std::scoped_lock lock(this->nodeMutex_);
    unsigned int healthyCount = 0;
    for (const auto& wPeople : this->presentPeople_) {
        auto people = wPeople.lock();
        if (people && people->getHealthStatus() == Healthy)
            healthyCount++;
    }
    return healthyCount;
}
unsigned int Node::getInfectedCount()
{
    std::scoped_lock lock(this->nodeMutex_);
    unsigned int infectedCound = 0;
    for (const auto& wPeople : this->presentPeople_) {
        auto people = wPeople.lock();
        if (people && (people->getHealthStatus() == Infected || people->getHealthStatus() == Sick))
            infectedCound++;
    }
    return infectedCound;
}

const std::vector<std::weak_ptr<zpr::Node>> Metro::getConnectedNodes()
{
    return this->connectedNodes_;
}

std::mutex& Node::getMutex()
{
    return this->nodeMutex_;
}

void Metro::addConnection(const std::shared_ptr<zpr::Node>& newNode)
{
    this->connectedNodes_.push_back(newNode);
}

void Place::addConnectingStation(const std::shared_ptr<zpr::Metro>& connectingStation)
{
    this->connectingStation_ = connectingStation;
}
const std::weak_ptr<zpr::Metro> Place::getConnecingStation()
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
    this->peopleLivingHere_.push_back(newPeople);
}
const std::vector<std::weak_ptr<zpr::Agent>> Home::getPeopleLivingHere()
{
    return this->peopleLivingHere_;
}

const std::vector<std::weak_ptr<zpr::Node>> Metro::getAllNeighbours()
{
    return getConnectedNodes();
}

const std::vector<std::weak_ptr<zpr::Node>> Place::getAllNeighbours()
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
    this->openingHour_ = newOpeningHour;
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

// visitors

void Metro::accept(zpr::NodeVisitor& visitor)
{
    visitor.visit(*this);
}

void Workplace::accept(zpr::NodeVisitor& visitor)
{
    visitor.visit(*this);
}

void Home::accept(zpr::NodeVisitor& visitor)
{
    visitor.visit(*this);
}

void Entertainment::accept(zpr::NodeVisitor& visitor)
{
    visitor.visit(*this);
}

void Place::accept(zpr::NodeVisitor& visitor)
{
    visitor.visit(*this);
}

}  // namespace zpr
