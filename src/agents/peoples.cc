#include "agents/peoples.h"

#include <memory>

#include "city/city.h"

namespace zpr {
std::weak_ptr<zpr::Home> Worker::getHome()
{
    return this->home_;
}
std::weak_ptr<zpr::Workplace> Worker::getWorkplace()
{
    return this->workplace_;
}

std::weak_ptr<zpr::Node> Worker::getPosition()
{
    return this->position_;
}
void Worker::setHome(const std::weak_ptr<zpr::Home> newHome)
{
    this->home_ = std::move(newHome);
}
void Worker::setWorkplace(const std::weak_ptr<zpr::Workplace> newWorkplace)
{
    this->workplace_ = std::move(newWorkplace);
}

void Worker::setPosition(std::weak_ptr<zpr::Node> newPosition)
{
    this->position_ = std::move(newPosition);
}
}  // namespace zpr