// Krzysztof Barałkiewicz

#include <string>
#include <vector>

#include "utils/SimSettings.h"

namespace zpr {
class SimulationLogger {
  public:
    struct Stats {
        unsigned int day;
        unsigned int hour;
        unsigned int people;
        unsigned int infected;
        unsigned int dead;
    };

    SimulationLogger(const std::string& filename = SimSettings::CSV_FILENAME);

    void saveToCSV(const Stats& stats);
    const std::string& getFilename() const;

    static void drawPlot(const std::string& filename = SimSettings::CSV_FILENAME);

  private:
    std::string filename_;
    bool hasWrittenHeader_ = false;

    void writeHeaderIfNeeded();
};
}  // namespace zpr
