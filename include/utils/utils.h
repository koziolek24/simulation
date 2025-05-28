// Maciej Kozlowski
// Podstawowe narzedzie do wczytywania informacji z pliku
// bedzie rozszerzane o inne formaty plikow - docelowo JSON
#ifndef UTILS
#define UTILS

#include <nlohmann/json.hpp>

#include "simulation/engine.h"
using json = nlohmann::json;

namespace zpr {
class reader {
  public:
    void readStructure(const std::string path, zpr::simulationEngine& engine);
    void readJsonStructure(const std::string path, zpr::simulationEngine& engine);
    void writeJsonStructure(const std::string path);
    void readProbabilities();

    void writeNodes(json& data);
    void writeWorkers(json& data);
    void writeConnections(json& data);
};
}  // namespace zpr

#endif
