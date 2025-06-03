// Maciej Kozlowski
// Glowny element symulacji, zawiera wszystkie wskazniki na wszystkie miejsca, oraz na wszystkich
// ludzi Bedzie odpowiadalo za wykonywanie akcji w czasie - co x czasu kazdy czlowiek bedzie
// wykonywal operacje, ktora moze wplywac na zarazliwosc jakiegos miejsca; engine bedzie
// odpowiedzialny za wykonywanie tych ruchow
//

#ifndef ENGINE_H
#define ENGINE_H

#include <atomic>
#include <map>
#include <memory>
#include <vector>

#include "agents/peoples.h"
#include "city/city.h"
#include "disease/disease.h"
#include "utils/SimStatsLogger.h"

namespace zpr {
class TimeManager {
  private:
    unsigned int hour_ = 6;
    unsigned int minute_ = 0;
    unsigned int day_ = 0;

  public:
    void incrementTime();
    unsigned int getHour();
    unsigned int getDay();
    unsigned int getMinute();
};

class GraphManager {
  private:
    std::vector<std::shared_ptr<zpr::Worker>> allWorkers_;
    std::vector<std::shared_ptr<zpr::Node>> allNodes_;
    std::map<std::string, std::shared_ptr<zpr::Node>> nodesByName_;

  public:
    void addWorker(std::shared_ptr<zpr::Worker> newWorker);
    void addNode(std::shared_ptr<zpr::Node> newNode);
    const std::vector<std::shared_ptr<zpr::Node>> getAllNodes();
    const std::vector<std::shared_ptr<zpr::Worker>> getAllWorkers();
    const std::shared_ptr<zpr::Worker> getWorkerById(unsigned int id);
    const std::shared_ptr<zpr::Node> getNodeById(unsigned int id);
    const std::shared_ptr<zpr::Node> getNodeByName(std::string nodeName);
};

class GraphCreator : public GraphManager {
  public:
    void connectMetroToMetro(std::string firstStation, std::string secondStation);
    void connectMetroToPlace(std::string stationName, std::shared_ptr<zpr::Place> place);
};

class GraphFinder {
  private:
    std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<zpr::Node>> cheapestPath_;

  public:
    const std::shared_ptr<zpr::Node> getNextNode(std::shared_ptr<zpr::Node> source,
                                                 std::shared_ptr<zpr::Node> target);
    void calculateCheapestPath(std::shared_ptr<zpr::Node> target);
    void calculateAllPaths(std::vector<std::shared_ptr<zpr::Node>> allNodes);
    const std::shared_ptr<zpr::Node> findNearestEntertainment(std::shared_ptr<zpr::Node> position);
};

class GraphTravel : public GraphFinder {
  public:
    void moveWorkerToTarget(std::shared_ptr<zpr::Worker> worker, std::shared_ptr<zpr::Node> target);
    // void moveWorkerHome(std::shared_ptr<zpr::Worker> worker);
    // void moveWorkerWorkplace(std::shared_ptr<zpr::Worker> worker);
};

class SimulationConfig {
  private:
    double severityEntertainment_;
    double severityWork_;
    unsigned int workStartHour_;
    unsigned int workEndHour_;
    unsigned int entertainmentStartHour_;
    unsigned int entertainmentEndHour_;
    unsigned int entertainmentStartToLeaveHour_;
    double entertainmentGoProbability_;
    double entertainmentLeaveProbability_;
    VirusManager virusManager_;
    std::atomic_uint deathPeopleCount_;

  public:
    SimulationConfig() : severityEntertainment_(0), severityWork_(0), deathPeopleCount_(0) {}
    void setSeverityEntertainment(double severityEntertainment);
    void setSeverityWork(double severityWork);
    void setWorkStartHour(unsigned int hour);
    void setEntertainmentStartHour(unsigned int hour);
    void setEntertainmentEndHour(unsigned int hour);
    void setEntertainmentStartToLeaveHour(unsigned int hour);
    void setEntertainmentGoProbability(double probability);
    void setEntertainmentLeaveProbability(double probability);
    void setVirusManager(VirusManager manager);

    unsigned int getWorkStartHour();
    unsigned int getEntertainmentStartHour();
    unsigned int getEntertainmentEndHour();
    unsigned int getEntertainmentStartToLeaveHour();
    double getWorkSeverity();
    double getEntertainmentSeverity();
    double getEntertainmentGoProbability();
    double getEntertainmentLeaveProbability();
    double getMortalityByID(unsigned int id);
    double getContagiousnessByID(unsigned int id);
    std::atomic_uint& getDeathPeopleCount();
    VirusManager getVirusManager();
    std::vector<std::shared_ptr<zpr::Symptom>> getSymptoms();
    std::shared_ptr<Symptom> getInitialSymptom();

    void increaseDeadPeopleCount(uint amount = 1);
};

class SimulationEngine : public GraphCreator, public TimeManager, public GraphTravel, public SimulationConfig {
  private:
    zpr::SimulationLogger simLogger_;
  public:
    void doAction();
    void doWorkerAction(std::shared_ptr<zpr::Worker> worker);
};
}  // namespace zpr
#endif
