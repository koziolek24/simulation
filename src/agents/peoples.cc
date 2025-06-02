#include "agents/peoples.h"

#include <memory>

#include "city/city.h"

namespace zpr {
const std::weak_ptr<zpr::Home> Worker::getHome()
{
    return this->home_;
}
const std::weak_ptr<zpr::Workplace> Worker::getWorkplace()
{
    return this->workplace_;
}

const std::weak_ptr<zpr::Node> Worker::getPosition()
{
    return this->position_;
}
void Worker::setHome(const std::weak_ptr<zpr::Home> newHome)
{
    this->home_ = newHome;
}
void Worker::setWorkplace(const std::weak_ptr<zpr::Workplace> newWorkplace)
{
    this->workplace_ = newWorkplace;
}

void Worker::setPosition(std::weak_ptr<zpr::Node> newPosition)
{
    this->position_ = newPosition;
}
}  // namespace zpr