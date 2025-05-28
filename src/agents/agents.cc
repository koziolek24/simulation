#include "agents/agents.h"

namespace zpr {
unsigned int Agent::getAge()
{
    return this->age_;
}
std::string Agent::getHealthStatus()
{
    std::string status = "";
    switch (this->health_) {
        case 0:
            status = "Healthy";
            break;
        case 1:
            status = "Infected";
            break;
        case 2:
            status = "Sick";
            break;
        case 3:
            status = "Dead";
            break;
    }
    return status;
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
}  // namespace zpr
