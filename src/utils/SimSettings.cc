// Krzysztof Barałkiewicz
#include "utils/SimSettings.h"

#include <algorithm>
#include <thread>

#include "utils/SimStatsLogger.h"

const std::string zpr::SimSettings::CSV_FILENAME = "simulation.csv";

zpr::SimSettings& zpr::SimSettings::getInstance()
{
    static SimSettings instance;
    return instance;
}

void zpr::SimSettings::setRunning(bool running)
{
    running_ = running;
}

bool zpr::SimSettings::isRunning() const
{
    return running_;
}

void zpr::SimSettings::setFinished(bool finished)
{
    finished_ = finished;
}

bool zpr::SimSettings::isFinished() const
{
    return finished_;
}

void zpr::SimSettings::setSpeed(int value)
{
    speed_ = std::clamp(value, 1, 128);
}

int zpr::SimSettings::getSpeed() const
{
    return speed_;
}

int zpr::SimSettings::getSpeedDelay() const
{
    return 1000 / speed_;
}

void zpr::SimSettings::increaseSpeed()
{
    setSpeed(this->speed_ * 2);
}

void zpr::SimSettings::decreaseSpeed()
{
    setSpeed(this->speed_ / 2);
}

void zpr::SimSettings::pauseResume()
{
    if (this->running_) {
        this->running_ = false;
        zpr::SimulationLogger::drawPlot();
        return;
    }
    this->running_ = true;
}
