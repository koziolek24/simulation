// Krzysztof Baralkiewicz
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <sys/types.h>

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "GraphLayout.h"
#include "city/city.h"
#include "simulation/engine.h"

namespace zpr {
class GraphWindow {
  public:
    GraphWindow() = default;
    bool init(const std::shared_ptr<SimulationEngine>& engine);
    int run();

    ~GraphWindow() = default;

  private:
    void processEvents();
    void render();

    void autoFitScale();

    std::vector<std::shared_ptr<zpr::Node>> nodes_;
    std::shared_ptr<zpr::SimulationEngine> engine_;
    std::unique_ptr<zpr::GraphLayout> layout_;

    sf::RenderWindow window_;
    sf::Font font_;
    std::optional<sf::Text> stats_;

    float view_;
    float scale_;
    sf::Vector2f offset_{0.f, 0.f};
    const float radius_ = 18.f;  // size of node

    // for displaying info in popup
    std::weak_ptr<zpr::Node> current_;
};
}  // namespace zpr
#endif  // GRAPHWINDOW_H
