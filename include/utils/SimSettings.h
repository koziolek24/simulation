// Krzysztof Barałkiewicz
#ifndef SIM_SETTINGS_H
#define SIM_SETTINGS_H

#include <ostream>
#include <sstream>
#include <string>

namespace zpr {
class SimSettings {
  public:
    static SimSettings& getInstance();
    static const std::string CSV_FILENAME;

    SimSettings(const SimSettings&) = delete;
    SimSettings& operator=(const SimSettings&) = delete;

    void setRunning(bool running);
    bool isRunning() const;

    void setFinished(bool finished);
    bool isFinished() const;

    void setSpeed(int value);
    int getSpeed() const;

    // returns milli seconds for delay between iterations
    int getSpeedDelay() const;

    void increaseSpeed();
    void decreaseSpeed();
    void pauseResume();

  private:
    SimSettings() = default;

    bool running_ = true;
    bool finished_ = false;
    int speed_ = 1;
};
}  // namespace zpr
#endif  // SIM_SETTINGS
