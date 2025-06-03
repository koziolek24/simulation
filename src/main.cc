#include <Python.h>
#include <simulation/engine.h>
#include <utils/utils.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <logging.hpp>
#include <memory>
#include <set>
#include <thread>

#include "agents/agents.h"
#include "disease/disease.h"
#include "gui/GraphWindow.h"
#include "utils/SimSettings.h"

int main()
{
    // for finding and linking python for ploting
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"" PYTHON_SITE "\")");

    init_logging();
    LOG_INFO  "ZPR SYMULACJA 2025");
    auto engine = std::make_shared<zpr::SimulationEngine>();
    auto virusManager = zpr::VirusManager();
    zpr::Reader reader;
    reader.writeJsonStructure("dane.json");
    // reader.writeViruses("virus.json");
    reader.readJsonStructure("dane.json", *engine);
    reader.readViruses("virus.json", virusManager);
    engine->calculateAllPaths(engine->getAllNodes());
    engine->setVirusManager(virusManager);
    zpr::GraphWindow gw;
    if (!gw.init(engine))
        return 1;

    std::vector<std::shared_ptr<zpr::Worker>> workers = engine->getAllWorkers();

    if (!workers.empty()) {
        unsigned int toInfect = 1;

        std::srand(static_cast<unsigned>(std::time(nullptr)));

        std::set<std::size_t> chosen;

        while (chosen.size() < toInfect) {
            std::size_t index = static_cast<std::size_t>(std::rand()) % workers.size();
            if (chosen.insert(index).second) {
                workers[index]->setHealthStatus(zpr::Infected);
                std::shared_ptr<zpr::Symptom> initialSymptom = engine->getInitialSymptom();
                workers[index]->getVirus()->addSymptom(initialSymptom);
            }
        }
    }

    auto& settings = zpr::SimSettings::getInstance();

    std::thread t([&]() {
        while (!settings.isFinished()) {
            if (settings.isRunning()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(settings.getSpeedDelay()));
                engine->doAction();
            }
        }
    });

    gw.run();
    settings.setFinished(true);
    t.join();
    return 0;
}
