#include "utils/utils.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <ostream>

#include "agents/peoples.h"
#include "city/city.h"
#include "simulation/engine.h"
using json = nlohmann::json;

using namespace zpr;

void reader::writeNodes(json& data)
{
    unsigned int id = 0;

    // Metros
    std::cout << "Write number of Metro stations" << std::endl;
    unsigned int metros;

    std::cin >> metros;
    for (unsigned int i = 0; i < metros; ++i) {
        std::cout << "Write name of the node" << std::endl;
        std::string name;
        std::cin >> name;
        data["Nodes"]["Metro"].push_back({{"id", id}, {"name", name}});
        ++id;
    }

    // Homes
    std::cout << "Write number of Homes" << std::endl;
    unsigned int homes;
    std::cin >> homes;
    for (unsigned int i = 0; i < homes; ++i) {
        std::cout << "Write name of the node" << std::endl;
        std::string name;
        std::cin >> name;
        std::cout << "Write name of connecting metro station" << std::endl;
        std::string metroName;
        std::cin >> metroName;
        data["Nodes"]["Home"].push_back(
            {{"id", id}, {"name", name}, {"connectedMetro", metroName}});
        ++id;
    }

    // Workplaces
    std::cout << "Write number of Workplaces" << std::endl;
    unsigned int workplaces;
    std::cin >> workplaces;
    for (unsigned int i = 0; i < workplaces; ++i) {
        std::cout << "Write name of the node" << std::endl;
        std::string name;
        std::cin >> name;
        std::cout << "Write opening and closing hour" << std::endl;
        unsigned int openingHour, closingHour;
        std::cin >> openingHour >> closingHour;
        std::cout << "Write name of connecting metro station" << std::endl;
        std::string metroName;
        std::cin >> metroName;
        data["Nodes"]["Workplace"].push_back({{"id", id},
                                              {"name", name},
                                              {"openingHour", openingHour},
                                              {"closingHour", closingHour},
                                              {"connectedMetro", metroName}});
        ++id;
    }

    std::cout << "Write number of entertainments" << std::endl;
    unsigned int entertainments;
    std::cin >> entertainments;
    for (unsigned int i = 0; i < entertainments; ++i) {
        std::cout << "Write name of the node" << std::endl;
        std::string name;
        std::cin >> name;
        std::cout << "Write name of connecting metro station" << std::endl;
        std::string metroName;
        std::cin >> metroName;
        data["Nodes"]["Entertainment"].push_back(
            {{"id", id}, {"name", name}, {"connectedMetro", metroName}});
        ++id;
    }
    data["MetroNumber"] = metros;
    data["HomeNumber"] = homes;
    data["WorkplaceNumber"] = workplaces;
    data["EntertainmentNumber"] = entertainments;
    data["nodeNumber"] = id;
    return;
}

void reader::writeConnections(json& data)
{
    std::cout << "Write numer of connections between metro stations" << std::endl;
    unsigned int connections;
    std::cin >> connections;
    for (unsigned int i = 0; i < connections; ++i) {
        std::cout << "Write names of connected stations" << std::endl;
        std::string stationA, stationB;
        std::cin >> stationA >> stationB;
        data["Connections"].push_back({{"stationA", stationA}, {"stationB", stationB}});
    }
    data["ConnectionNumber"] = connections;
    return;
}

void reader::writeWorkers(json& data)
{
    unsigned int id = 0;
    std::cout << "Write numer of Workers" << std::endl;
    unsigned int workers;
    std::cin >> workers;
    for (unsigned int i = 0; i < workers; ++i) {
        std::cout << "Write age of worker" << std::endl;
        unsigned int age;
        std::cin >> age;
        std::cout << "Write name of home" << std::endl;
        std::string home;
        std::cin >> home;
        std::cout << "Write name of workplace" << std::endl;
        std::string workplace;
        std::cin >> workplace;
        data["Workers"]["Worker"].push_back(
            {{"id", id}, {"age", age}, {"home", home}, {"workplace", workplace}});
        ++id;
    }
    // if added another class of worker write them here
    //
    //
    data["WorkerNumber"] = workers;
    data["WorkerNumber"] = id;
    return;
}

void reader::writeJsonStructure(const std::string path)
{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    json data;
    data["NodeNumber"] = 0;
    data["WorkerNumber"] = 0;
    data["Nodes"] = {{"Home", {}}, {"Workplace", {}}, {"Metro", {}}, {"Entertainment", {}}};
    data["Connections"] = {};
    data["Workers"] = {{"Worker", {}}};
    reader::writeNodes(data);
    reader::writeConnections(data);
    reader::writeWorkers(data);
    file << data.dump(4) << std::endl;
    file.close();
}

void reader::readJsonStructure(const std::string path, zpr::simulationEngine& engine)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "File doesn't exit" << std::endl;
        return;
    }
    json data = json::parse(file);

    // metro Stations
    int metros = data["MetroNumber"];
    for (int i = 0; i < metros; ++i) {
        int id = data["Nodes"]["Metro"][(long unsigned int)(i)]["id"];
        std::string name = data["Nodes"]["Metro"][(long unsigned int)(i)]["name"];
        zpr::Metro metroStation = zpr::Metro((unsigned int)(id), name);
        engine.addNode(std::make_shared<zpr::Metro>(metroStation));
    }

    // homes
    int homes = data["HomeNumber"];
    for (int i = 0; i < homes; ++i) {
        unsigned int id = data["Nodes"]["Home"][(long unsigned int)(i)]["id"];
        std::string name = data["Nodes"]["Home"][(long unsigned int)(i)]["name"];
        std::string metroName = data["Nodes"]["Home"][(long unsigned int)(i)]["connectedMetro"];
        zpr::Home home = zpr::Home(id, name);
        std::shared_ptr<zpr::Home> sHome = std::make_shared<zpr::Home>(home);
        engine.addNode(sHome);
        engine.connectMetroToPlace(metroName, sHome);
    }
    int workplaces = data["WorkplaceNumber"];
    for (int i = 0; i < workplaces; ++i) {
        unsigned int id = data["Nodes"]["Workplace"][(long unsigned int)(i)]["id"];
        std::string name = data["Nodes"]["Workplace"][(long unsigned int)(i)]["name"];
        std::string metroName =
            data["Nodes"]["Workplace"][(long unsigned int)(i)]["connectedMetro"];
        unsigned int openingHour =
            data["Nodes"]["Workplace"][(long unsigned int)(i)]["openingHour"];
        unsigned int closingHour =
            data["Nodes"]["Workplace"][(long unsigned int)(i)]["closingHour"];
        zpr::Workplace workplace = zpr::Workplace(id, name, openingHour, closingHour);
        std::shared_ptr<zpr::Workplace> sWorkplace = std::make_shared<zpr::Workplace>(workplace);
        engine.addNode(sWorkplace);
        engine.connectMetroToPlace(metroName, sWorkplace);
    }

    int entertainments = data["EntertainmentNumber"];
    for (int i = 0; i < entertainments; ++i) {
        unsigned int id = data["Nodes"]["Entertainment"][(long unsigned int)(i)]["id"];
        std::string name = data["Nodes"]["Entertainment"][(long unsigned int)(i)]["name"];
        std::string metroName =
            data["Nodes"]["Entertainment"][(long unsigned int)(i)]["connectedMetro"];
        zpr::Entertainment entertainment = zpr::Entertainment(id, name);
        std::shared_ptr<zpr::Entertainment> sEntertainment =
            std::make_shared<zpr::Entertainment>(entertainment);
        engine.addNode(sEntertainment);
        engine.connectMetroToPlace(metroName, sEntertainment);
    }

    int connections = data["ConnectionNumber"];
    for (int i = 0; i < connections; ++i) {
        std::string stationA = data["Connections"][(long unsigned int)(i)]["stationA"];
        std::string stationB = data["Connections"][(long unsigned int)(i)]["stationB"];
        engine.connectMetroToMetro(stationA, stationB);
    }

    // Workers
    int workers = data["WorkerNumber"];
    for (int i = 0; i < workers; ++i) {
        std::string home = data["Workers"]["Worker"][(long unsigned int)(i)]["home"];
        std::string workplace = data["Workers"]["Worker"][(long unsigned int)(i)]["workplace"];
        unsigned int id = data["Workers"]["Worker"][(long unsigned int)(i)]["id"];
        unsigned int age = data["Workers"]["Worker"][(long unsigned int)(i)]["age"];
        std::shared_ptr<zpr::Home> sHome =
            std::dynamic_pointer_cast<zpr::Home>(engine.getNodeByName(home));
        std::shared_ptr<zpr::Workplace> sWorkplace =
            std::dynamic_pointer_cast<zpr::Workplace>(engine.getNodeByName(workplace));
        zpr::Worker worker = zpr::Worker(id, age, sHome, sHome, sWorkplace);
        std::shared_ptr<zpr::Worker> sWorker = std::make_shared<zpr::Worker>(worker);
        engine.addWorker(sWorker);
        sHome->addPeopleLivingHere(sWorker);
        sHome->addPeople(sWorker);
    }
    return;
}