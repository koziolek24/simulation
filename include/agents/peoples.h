// Maciej Kozłowski

#ifndef PEOPLES_H
#define PEOPLES_H
#include <memory>

#include "agents.h"
#include "city/city.h"

namespace zpr {
class Worker : public Agent {
  private:
    std::weak_ptr<zpr::Home> home_;
    std::weak_ptr<zpr::Workplace> workplace_;
    std::weak_ptr<zpr::Node> position_;

  public:
    Worker(unsigned int id, unsigned int age, std::shared_ptr<zpr::Node> position,
           std::shared_ptr<zpr::Home> home, std::shared_ptr<zpr::Workplace> workplace)
        : Agent(id, age), home_(home), workplace_(workplace), position_(position)
    {
    }
    std::weak_ptr<zpr::Home> getHome();
    std::weak_ptr<zpr::Workplace> getWorkplace();
    std::weak_ptr<zpr::Node> getPosition();
    void setHome(const std::weak_ptr<zpr::Home> newHome);
    void setWorkplace(const std::weak_ptr<zpr::Workplace> newWorkplace);
    void setPosition(std::weak_ptr<zpr::Node> newPosition);
};
}  // namespace zpr

#endif