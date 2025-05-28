// Krzysztof Baralkiewicz

#ifndef GRAPHLAYOUT_H
#define GRAPHLAYOUT_H

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

#include "city/city.h"

namespace zpr {
class GraphLayout {
  public:
    GraphLayout(float areaSide, float spring = 80.f, float cooling = 0.96f);

    bool iterate(const std::vector<std::shared_ptr<zpr::Node>>& nodes);

  private:
    float side_{}, k_{}, cool_{};
};
}  // namespace zpr

#endif  // GRAPHLAYOUT_H
