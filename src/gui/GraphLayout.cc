// Krzysztof Barałkiewicz
#include "gui/GraphLayout.h"

#include <algorithm>
#include <cmath>
#include <memory>

#include "city/city.h"

static sf::Vector2f normalize(sf::Vector2f v, float newLen = 1.f)
{
    float len = std::hypot(v.x, v.y) + 1e-6f;
    return {v.x / len * newLen, v.y / len * newLen};
}

zpr::GraphLayout::GraphLayout(float side, float spring, float cooling)
    : side_(side), k_(spring), cool_(cooling)
{
}

bool zpr::GraphLayout::iterate(const std::vector<std::shared_ptr<zpr::Node>>& nodes)
{
    const float repulse = k_ * k_;
    bool moving = false;

    for (auto& a : nodes)
        for (auto& b : nodes)
            if (a != b) {
                sf::Vector2f d = a->pos_ - b->pos_;
                float dist2 = d.x * d.x + d.y * d.y + 1e-4f;
                a->vel_ += normalize(d) * (repulse / dist2);
            }

    for (auto& n : nodes)
        for (auto& w : n->getAllNeighbours()) {
            sf::Vector2f d = n->pos_ - w.lock()->pos_;
            float dist = std::hypot(d.x, d.y);
            sf::Vector2f f = normalize(d) * (dist - k_);
            n->vel_ -= f;
            w.lock()->vel_ += f;
        }

    float half = side_ / 2.f;
    for (auto& n : nodes) {
        n->pos_ += n->vel_ * 0.02f;
        n->vel_ *= cool_;

        n->pos_.x = std::clamp(n->pos_.x, -half, half);
        n->pos_.y = std::clamp(n->pos_.y, -half, half);

        moving |= (std::abs(n->vel_.x) > 0.01f || std::abs(n->vel_.y) > 0.01f);
    }
    return moving;
}
