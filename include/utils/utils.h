// Maciej Kozlowski
// Podstawowe narzedzie do wczytywania informacji z pliku
// bedzie rozszerzane o inne formaty plikow - docelowo JSON
#ifndef UTILS
#define UTILS

#include <nlohmann/json.hpp>

#include "disease/disease.h"
#include "simulation/engine.h"
using json = nlohmann::json;

namespace zpr {
class Reader {
  public:
    void readStructure(const std::string path, zpr::SimulationEngine& engine);
    void readJsonStructure(const std::string path, zpr::SimulationEngine& engine);
    void writeJsonStructure(const std::string path);
    void readProbabilities();
    void readViruses(const std::string path, zpr::VirusManager& virusManager);

    void writeNodes(json& data);
    void writeWorkers(json& data);
    void writeConnections(json& data);
    void writeViruses(const std::string path);
};
}  // namespace zpr

#endif
