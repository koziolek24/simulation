// Krzysztof Barałkiewicz#include "SimulationLogger.hpp"
//
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include "matplotlib-cpp/matplotlib-cpp.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <utils/SimStatsLogger.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils/SimSettings.h"

namespace plt = matplotlibcpp;

zpr::SimulationLogger::SimulationLogger(const std::string& filename) : filename_(filename)
{
    writeHeaderIfNeeded();
}

void zpr::SimulationLogger::writeHeaderIfNeeded()
{
    std::ofstream out(filename_);
    out << "Day,Hour,People,Infected,Dead\n";
    hasWrittenHeader_ = true;
}

void zpr::SimulationLogger::saveToCSV(const Stats& stats)
{
    std::ofstream out(filename_, std::ios::app);
    out << stats.day << "," << stats.hour << "," << stats.people << "," << stats.infected << ","
        << stats.dead << "\n";
}

const std::string& zpr::SimulationLogger::getFilename() const
{
    return filename_;
}

void zpr::SimulationLogger::drawPlot(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    std::string line;
    std::getline(file, line);  // skip header

    std::vector<double> time, people, infected, healthy, dead;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string cell;
        int day = 0, hour = 0, p = 0, i = 0, d = 0;
        std::getline(ss, cell, ',');
        day = std::stoi(cell);
        std::getline(ss, cell, ',');
        hour = std::stoi(cell);
        std::getline(ss, cell, ',');
        p = std::stoi(cell);
        std::getline(ss, cell, ',');
        i = std::stoi(cell);
        std::getline(ss, cell, ',');
        d = std::stoi(cell);

        double t = day + hour / 24.0;
        time.push_back(t);
        people.push_back(p);
        infected.push_back(i);
        healthy.push_back(static_cast<double>(p - i));
        dead.push_back(d);
    }

    plt::figure_size(1200, 600);
    plt::named_plot("People", time, people, "b-");
    plt::named_plot("Infected", time, infected, "r-");
    plt::named_plot("Healthy", time, healthy, "g-");
    plt::named_plot("Dead", time, dead, "k-");

    plt::xlabel("Day");
    plt::ylabel("Count");
    plt::title("ZPR25L Simulation Statistics");
    plt::grid(true);
    plt::legend();

    plt::save("plot.png");
}
