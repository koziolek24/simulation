#include "visitors/ShapeMakerVisitor.h"

#include <SFML/Graphics.hpp>

#include "city/city.h"

namespace zpr {

ShapeMakerVisitor::ShapeMakerVisitor(float radius) : radius_(radius) {}

void ShapeMakerVisitor::visit(Metro&)
{
    auto s = std::make_unique<sf::CircleShape>(radius_);
    s->setOrigin(radius_, radius_);
    shape_ = std::move(s);
}

void ShapeMakerVisitor::visit(Home&)
{
    auto s = std::make_unique<sf::RectangleShape>(sf::Vector2f(radius_ * 2.f, radius_ * 2.f));
    s->setOrigin(radius_, radius_);
    shape_ = std::move(s);
}

void ShapeMakerVisitor::visit(Workplace&)
{
    auto s = std::make_unique<sf::ConvexShape>(3);
    s->setPoint(0, {0.f, -radius_});
    s->setPoint(1, {radius_, radius_});
    s->setPoint(2, {-radius_, radius_});
    s->setOrigin(0.f, 0.f);
    shape_ = std::move(s);
}

void ShapeMakerVisitor::visit(Entertainment&)
{
    auto s = std::make_unique<sf::CircleShape>(radius_, 6);  // hexagon
    s->setOrigin(radius_, radius_);
    shape_ = std::move(s);
}

void ShapeMakerVisitor::visit(Node&)
{
    auto s = std::make_unique<sf::CircleShape>(radius_, 8);  // octagon
    s->setOrigin(radius_, radius_);
    shape_ = std::move(s);
}

sf::Shape& ShapeMakerVisitor::getShape()
{
    if (!shape_)
        throw std::runtime_error("Shape not initialized");
    return *shape_;
}

}  // namespace zpr
