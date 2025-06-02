#include "utils/utils.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <ostream>
#include <vector>

#include "agents/peoples.h"
#include "city/city.h"
#include "disease/disease.h"
#include "simulation/engine.h"
using json = nlohmann::json;

using namespace zpr;

void Reader::writeNodes(json& data)
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
}

void Reader::writeConnections(json& data)
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
}

void Reader::writeWorkers(json& data)
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
    data["WorkerNumber"] = id;
}

void Reader::writeJsonStructure(const std::string path)
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
    std::cin >> data["SeverityEntertainment"];
    std::cin >> data["SeverityWork"];
    std::cin >> data["WorkStartHour"];
    std::cin >> data["EntertainmentStartHour"];
    std::cin >> data["EntertainmentEndHour"];
    std::cin >> data["EntertainmentStartToLeaveHour"];
    std::cin >> data["EntertainmentGoProbability"];
    std::cin >> data["EntertainmentLeaveProbability"];
    std::cout << "Next data" << std::endl;
    Reader::writeNodes(data);
    Reader::writeConnections(data);
    Reader::writeWorkers(data);
    file << data.dump(4) << std::endl;
    file.close();
}

void Reader::readJsonStructure(const std::string path, zpr::SimulationEngine& engine)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "File doesn't exist" << std::endl;
        return;
    }
    json data = json::parse(file);

    double severityEntertainment = data["SeverityEntertainment"];
    double severityWork = data["SeverityWork"];
    unsigned int workStartHour = data["WorkStartHour"];
    unsigned int entertainmentStartHour = data["EntertainmentStartHour"];
    unsigned int entertainmentEndHour = data["EntertainmentEndHour"];
    unsigned int entertainmentStartToLeaveHour = data["EntertainmentStartToLeaveHour"];
    double entertainmentGoProbability = data["EntertainmentGoProbability"];
    double entertainmentLeaveProbability = data["EntertainmentLeaveProbability"];
    engine.setSeverityEntertainment(severityEntertainment);
    engine.setSeverityWork(severityWork);
    engine.setWorkStartHour(workStartHour);
    engine.setEntertainmentStartHour(entertainmentStartHour);
    engine.setEntertainmentEndHour(entertainmentEndHour);
    engine.setEntertainmentStartToLeaveHour(entertainmentStartToLeaveHour);
    engine.setEntertainmentGoProbability(entertainmentGoProbability);
    engine.setEntertainmentLeaveProbability(entertainmentLeaveProbability);
    // Metro
    int metros = data["MetroNumber"];
    for (int i = 0; i < metros; ++i) {
        unsigned int id = data["Nodes"]["Metro"][static_cast<std::size_t>(i)]["id"];
        std::string name = data["Nodes"]["Metro"][static_cast<std::size_t>(i)]["name"];
        engine.addNode(std::make_shared<zpr::Metro>(id, name));
    }

    // Home
    int homes = data["HomeNumber"];
    for (int i = 0; i < homes; ++i) {
        unsigned int id = data["Nodes"]["Home"][static_cast<std::size_t>(i)]["id"];
        std::string name = data["Nodes"]["Home"][static_cast<std::size_t>(i)]["name"];
        std::string metroName =
            data["Nodes"]["Home"][static_cast<std::size_t>(i)]["connectedMetro"];
        auto sHome = std::make_shared<zpr::Home>(id, name);
        engine.addNode(sHome);
        engine.connectMetroToPlace(metroName, sHome);
    }

    // Workplace
    int workplaces = data["WorkplaceNumber"];
    for (int i = 0; i < workplaces; ++i) {
        unsigned int id = data["Nodes"]["Workplace"][static_cast<std::size_t>(i)]["id"];
        std::string name = data["Nodes"]["Workplace"][static_cast<std::size_t>(i)]["name"];
        std::string metroName =
            data["Nodes"]["Workplace"][static_cast<std::size_t>(i)]["connectedMetro"];
        unsigned int openingHour =
            data["Nodes"]["Workplace"][static_cast<std::size_t>(i)]["openingHour"];
        unsigned int closingHour =
            data["Nodes"]["Workplace"][static_cast<std::size_t>(i)]["closingHour"];
        auto sWorkplace = std::make_shared<zpr::Workplace>(id, name, openingHour, closingHour);
        engine.addNode(sWorkplace);
        engine.connectMetroToPlace(metroName, sWorkplace);
    }

    // Entertainment
    int entertainments = data["EntertainmentNumber"];
    for (int i = 0; i < entertainments; ++i) {
        unsigned int id = data["Nodes"]["Entertainment"][static_cast<std::size_t>(i)]["id"];
        std::string name = data["Nodes"]["Entertainment"][static_cast<std::size_t>(i)]["name"];
        std::string metroName =
            data["Nodes"]["Entertainment"][static_cast<std::size_t>(i)]["connectedMetro"];
        auto sEntertainment = std::make_shared<zpr::Entertainment>(id, name);
        engine.addNode(sEntertainment);
        engine.connectMetroToPlace(metroName, sEntertainment);
    }

    // Connections
    int connections = data["ConnectionNumber"];
    for (int i = 0; i < connections; ++i) {
        std::string stationA = data["Connections"][static_cast<std::size_t>(i)]["stationA"];
        std::string stationB = data["Connections"][static_cast<std::size_t>(i)]["stationB"];
        engine.connectMetroToMetro(stationA, stationB);
    }

    // Workers
    int workers = data["WorkerNumber"];
    for (int i = 0; i < workers; ++i) {
        unsigned int id = data["Workers"]["Worker"][static_cast<std::size_t>(i)]["id"];
        unsigned int age = data["Workers"]["Worker"][static_cast<std::size_t>(i)]["age"];
        std::string home = data["Workers"]["Worker"][static_cast<std::size_t>(i)]["home"];
        std::string workplace = data["Workers"]["Worker"][static_cast<std::size_t>(i)]["workplace"];
        auto sHome = std::dynamic_pointer_cast<zpr::Home>(engine.getNodeByName(home));
        auto sWorkplace =
            std::dynamic_pointer_cast<zpr::Workplace>(engine.getNodeByName(workplace));
        auto sWorker = std::make_shared<zpr::Worker>(id, age, sHome, sHome, sWorkplace);
        engine.addWorker(sWorker);
        sHome->addPeopleLivingHere(sWorker);
        sHome->addPeople(sWorker);
    }
}

void Reader::writeViruses(const std::string path)
{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    json data;
    std::cout << "Write number of symptoms" << std::endl;
    unsigned int symptoms;
    std::cin >> symptoms;
    data["SymptomsNumber"] = symptoms;
    unsigned int symptomID = 1;
    for (unsigned int i = 0; i < symptoms; ++i) {
        std::cout << "Write name of the symptom" << std::endl;
        std::string symptomName;
        std::cin >> symptomName;
        std::cout << "Write symptom severity" << std::endl;
        double severity;
        std::cin >> severity;
        std::cout << "Write number of dependables" << std::endl;
        unsigned int dependableNumber;
        std::cin >> dependableNumber;
        std::vector<std::string> dependables;
        for (unsigned int j = 0; j < dependableNumber; ++j) {
            std::cout << "Write name of dependable" << std::endl;
            std::string dependableName;
            std::cin >> dependableName;
            dependables.push_back(dependableName);
        }
        unsigned int dependableDataCount = 1 << dependableNumber;
        std::vector<double> evolutionProbabilities;
        for (unsigned int j = 0; j < dependableDataCount; ++j) {
            std::cout << "Write probability";
            double probability;
            std::cin >> probability;
            evolutionProbabilities.push_back(probability);
        }
        data["Symptoms"].push_back({{"name", symptomName},
                                    {"id", symptomID},
                                    {"severity", severity},
                                    {"DependableNumber", dependableNumber},
                                    {"Dependables", dependables},
                                    {"EvolutionProbabilities", evolutionProbabilities}});
        symptomID <<= 1;
    }
    unsigned int initialSymptomNumber;
    std::cin >> initialSymptomNumber;
    std::vector<std::string> initialSymptoms;
    data["InitialSymptomsNumber"] = initialSymptomNumber;
    for (unsigned int i = 0; i < initialSymptomNumber; ++i) {
        std::string symptom;
        std::cin >> symptom;
        initialSymptoms.push_back(symptom);
    }
    data["InitialSymptoms"] = initialSymptoms;

    unsigned int dataCount = 1 << symptoms;
    data["Contagiousness"].push_back({{"id", 0}, {"contagiousness", (double)0}});
    data["Mortality"].push_back({{"id", 0}, {"mortality", (double)0}});
    for (unsigned int i = 1; i < dataCount; ++i) {
        std::cout << "Write contagiousness" << std::endl;
        double contagiousness;
        std::cin >> contagiousness;
        std::cout << "Write mortality" << std::endl;
        double mortality;
        std::cin >> mortality;
        data["Contagiousness"].push_back({{"id", i}, {"contagiousness", contagiousness}});
        data["Mortality"].push_back({{"id", i}, {"mortality", mortality}});
    }
    file << data.dump(4) << std::endl;
    file.close();
    return;
}

void Reader::readViruses(const std::string path, zpr::VirusManager& virusManager)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "File doesn't exit" << std::endl;
        return;
    }
    json data = json::parse(file);
    unsigned int symptomsNumber = data["SymptomsNumber"];
    for (unsigned int i = 0; i < symptomsNumber; ++i) {
        unsigned int id = data["Symptoms"][i]["id"];
        std::string name = data["Symptoms"][i]["name"];
        double severity = data["Symptoms"][i]["severity"];
        std::shared_ptr<zpr::Symptom> sSymptom =
            std::make_shared<zpr::Symptom>(zpr::Symptom(id, name, severity));
        virusManager.addSymptom(sSymptom);
        unsigned int dependableNumber = data["Symptoms"][i]["DependableNumber"];
        for (unsigned int j = 0; j < dependableNumber; ++j) {
            std::string newDependable = data["Symptoms"][i]["Dependables"][j];
            sSymptom->addDependable(newDependable);
        }
        unsigned int dependableDataCount = 1 << dependableNumber;
        for (unsigned int j = 0; j < dependableDataCount; ++j) {
            double probability = data["Symptoms"][i]["EvolutionProbabilities"][j];
            sSymptom->addEvolveProbability(probability);
        }
    }
    unsigned int dataCount = 1 << symptomsNumber;
    for (unsigned int i = 0; i < dataCount; ++i) {
        double mortality = data["Mortality"][i]["mortality"];
        double contagiousness = data["Contagiousness"][i]["contagiousness"];
        virusManager.addMortality(mortality);
        virusManager.addContagiousness(contagiousness);
    }
    unsigned int initialSymptomsNumber = data["InitialSymptomsNumber"];
    for (unsigned int i = 0; i < initialSymptomsNumber; ++i) {
        std::string initial = data["InitialSymptoms"][i];
        virusManager.addInitialSymptom(virusManager.getSymptomByName(initial));
    }
    return;
}
