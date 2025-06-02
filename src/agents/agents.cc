#include "agents/agents.h"

namespace zpr {
unsigned int Agent::getAge()
{
    return this->age_;
}
healthStatus Agent::getHealthStatus()
{
    return this->health_;
}
void Agent::setHealthStatus(healthStatus health)
{
    this->health_ = health;
}

unsigned int Agent::getID()
{
    return this->id_;
}

target Agent::getTarget()
{
    return this->target_;
}

void Agent::setTarget(target newTarget)
{
    this->target_ = newTarget;
}

void Agent::setAge(unsigned int newAge)
{
    this->age_ = newAge;
}

void Agent::setID(unsigned int newID)
{
    this->id_ = newID;
}

std::shared_ptr<Virus>& Agent::getVirus()
{
    return this->virus_;
}
}  // namespace zpr
