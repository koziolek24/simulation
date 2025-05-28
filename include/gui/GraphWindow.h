// Krzysztof Baralkiewicz
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <sys/types.h>

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "GraphLayout.h"
#include "city/city.h"

namespace zpr {
class GraphWindow {
  public:
    GraphWindow() = default;
    bool init(const std::vector<std::shared_ptr<zpr::Node>>& nodes, float viewSize = 1600.f,
              float scale = 1.4f, float spring = 6.f);
    int run();

    ~GraphWindow() = default;

  private:
    void processEvents();
    void render();

    void autoFitScale(float margin = 24.f);

    std::vector<std::shared_ptr<zpr::Node>> nodes_;
    zpr::GraphLayout layout_{400.f};

    sf::RenderWindow window_;
    sf::Font font_;
    std::optional<sf::Text> stats_;

    float view_ = 400.f;
    float scale_ = 1.4f;
    sf::Vector2f offset_{0.f, 0.f};
    const float radius_ = 18.f;

    // for displaying info in popup
    std::weak_ptr<zpr::Node> current_;
};
}  // namespace zpr
#endif  // GRAPHWINDOW_H
