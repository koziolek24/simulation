#include <iostream>

// #include "gui/GraphWindow.h"
#include "simulation/engine.h"
#include "utils/utils.h"

int main()
{
    std::cout << "ZPR SYMULACJA 2025" << std::endl;
    zpr::simulationEngine engine;
    zpr::reader reader;
    // reader.writeJsonStructure("../dane.json");
    reader.readJsonStructure("/home/maciej/code/cpp/zpr25l/dane.json", engine);
    engine.calculateAllPaths();
    for (int i = 0; i < 400; ++i) {
        engine.doAction();
        unsigned int hour = engine.getHour();
        unsigned int minute = engine.getMinute();
        if (hour == 7 or hour == 8 or hour == 16 or
            hour == 17 or (hour >= 20 and minute%30 == 0)) {
            std::cout << engine.getDay() << " " << engine.getHour() << ":" << engine.getMinute()
                      << std::endl;
            for (auto node : engine.getAllNodes()) {
                if (node->getPeopleCount() != 0)
                    std::cout << node->getName() << " " << node->getNodeType() << " "
                              << node->getPeopleCount() << std::endl;
            }
        }
    }
    // zpr::GraphWindow gw;
    // if (!gw.init(engine.getAllNodes()))
    //     return 1;
    // return gw.run();
}

// A G
// A B C D E F G
