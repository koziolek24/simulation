// Maciej Kozlowski
// Glowny element symulacji, zawiera wszystkie wskazniki na wszystkie miejsca, oraz na wszystkich
// ludzi Bedzie odpowiadalo za wykonywanie akcji w czasie - co x czasu kazdy czlowiek bedzie
// wykonywal operacje, ktora moze wplywac na zarazliwosc jakiegos miejsca; engine bedzie
// odpowiedzialny za wykonywanie tych ruchow
//

#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <memory>

#include "agents/peoples.h"
#include "city/city.h"

namespace zpr {
class simulationEngine {
  private:
    std::vector<std::shared_ptr<zpr::Node>> allNodes_;
    std::vector<std::shared_ptr<zpr::Worker>> allWorkers_;
    std::map<std::pair<unsigned int, unsigned int>, std::weak_ptr<zpr::Node>> cheapestPath_;
    unsigned int hour_ = 0;
    unsigned int minute_ = 0;
    unsigned int day_ = 0;

  public:
    simulationEngine() {}
    void addWorker(std::shared_ptr<zpr::Worker> newWorker);
    void addNode(std::shared_ptr<zpr::Node> newNode);
    std::vector<std::shared_ptr<zpr::Node>> getAllNodes();
    std::vector<std::shared_ptr<zpr::Worker>> getAllWorkers();
    std::shared_ptr<zpr::Worker> getWorkerById(unsigned int id);
    std::shared_ptr<zpr::Node> getNodeById(unsigned int id);
    std::weak_ptr<zpr::Node> getNextNode(std::weak_ptr<zpr::Node> source,
                                         std::weak_ptr<zpr::Node> target);
    std::shared_ptr<zpr::Node> findNode(std::string nodeName);
    void connectMetroToMetro(std::string firstStation, std::string secondStation);
    void connectMetroToPlace(std::string stationName, std::shared_ptr<zpr::Place> place);
    void calculateCheapestPath(std::shared_ptr<zpr::Node> node);
    void calculateAllPaths();
    std::weak_ptr<zpr::Node> findNearestEntertainment(std::weak_ptr<zpr::Node> position);
    void moveWorkerToTarget(std::shared_ptr<zpr::Worker> worker, std::weak_ptr<zpr::Node> target);
    void moveWorkerHome(std::shared_ptr<zpr::Worker> worker);
    void moveWorkerWorkplace(std::shared_ptr<zpr::Worker> worker);
    void incrementTime();
    unsigned int getHour();
    unsigned int getDay();
    unsigned int getMinute();
    void doAction();
    void doWorkerAction(std::shared_ptr<zpr::Worker> worker);
};
}  // namespace zpr
#endif
