// Maciej Kozlowski
/* Plik zawiera implementacja Agenta - na ten moment podstawową
 * Agent zawiera informacje o stanie zdrowia i wieku
 * Klasa będzie rozszerzana i będą implementowane klasy dziedziczone po niej.
 *
 * Agent będzie przemieszczał się po sieci miasta samodzielnie i wpływał w momencie x na
 * prawdopodobienstwo zarazenia sie w zaleznosci od ilosci osob w danym punkcie
 */
#ifndef AGENTS_H
#define AGENTS_H

#include <memory>

#include "disease/disease.h"
namespace zpr {
enum healthStatus { Healthy, Infected, Sick, Dead };
enum target { HomeTarget, WorkplaceTarget, EntertainmentTarget };
class Agent {
  private:
    unsigned int id_;
    unsigned int age_;
    enum healthStatus health_;
    target target_;
    std::shared_ptr<Virus> virus_;

  public:
    Agent(unsigned int id, unsigned int age)
        : id_(id),
          age_(age),
          health_(Healthy),
          target_(HomeTarget),
          virus_(std::make_shared<zpr::Virus>())
    {
    }
    healthStatus getHealthStatus();
    unsigned int getAge();
    unsigned int getID();
    void setHealthStatus(healthStatus health);
    void setAge(unsigned int newAge);
    void setID(unsigned int newID);
    target getTarget();
    void setTarget(target newTarget);
    std::shared_ptr<Virus>& getVirus();
};
}  // namespace zpr
#endif
