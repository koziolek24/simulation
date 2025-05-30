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
#include <vector>

#include "agents/peoples.h"
#include "city/city.h"

namespace zpr {
class timeManager {
  private:
    unsigned int hour_ = 0;
    unsigned int minute_ = 0;
    unsigned int day_ = 0;

  public:
    void incrementTime();
    unsigned int getHour();
    unsigned int getDay();
    unsigned int getMinute();
};

class graphManager {
  private:
    std::vector<std::shared_ptr<zpr::Worker>> allWorkers_;
    std::vector<std::shared_ptr<zpr::Node>> allNodes_;

  public:
    void addWorker(std::shared_ptr<zpr::Worker> newWorker);
    void addNode(std::shared_ptr<zpr::Node> newNode);
    std::vector<std::shared_ptr<zpr::Node>> getAllNodes();
    std::vector<std::shared_ptr<zpr::Worker>> getAllWorkers();
    std::shared_ptr<zpr::Worker> getWorkerById(unsigned int id);
    std::shared_ptr<zpr::Node> getNodeById(unsigned int id);
    std::shared_ptr<zpr::Node> getNodeByName(std::string nodeName);
};

class graphCreator : public graphManager {
  public:
    void connectMetroToMetro(std::string firstStation, std::string secondStation);
    void connectMetroToPlace(std::string stationName, std::shared_ptr<zpr::Place> place);
};

class graphFinder {
  private:
    std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<zpr::Node>> cheapestPath_;

  public:
    std::shared_ptr<zpr::Node> getNextNode(std::shared_ptr<zpr::Node> source,
                                         std::shared_ptr<zpr::Node> target);
    void calculateCheapestPath(std::shared_ptr<zpr::Node> node);
    void calculateAllPaths(std::vector<std::shared_ptr<zpr::Node>> allNodes);
    std::shared_ptr<zpr::Node> findNearestEntertainment(std::shared_ptr<zpr::Node> position);
    std::map<std::pair<unsigned int, unsigned int>, unsigned int> getCheapestPath();
};

class graphTravel : public graphFinder {
  public:
    void moveWorkerToTarget(std::shared_ptr<zpr::Worker> worker, std::shared_ptr<zpr::Node> target);
    void moveWorkerHome(std::shared_ptr<zpr::Worker> worker);
    void moveWorkerWorkplace(std::shared_ptr<zpr::Worker> worker);
};

class simulationEngine : public graphCreator, public timeManager, public graphTravel {
  public:
    simulationEngine() {}
    void doAction();
    void doWorkerAction(std::shared_ptr<zpr::Worker> worker);
};
}  // namespace zpr
#endif
