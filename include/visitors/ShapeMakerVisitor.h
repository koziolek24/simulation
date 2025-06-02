#ifndef SHAPEMAKERVISITOR_H
#define SHAPEMAKERVISITOR_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "NodeVisitor.h"
#include "SFML/Graphics/Shape.hpp"

namespace zpr {

class ShapeMakerVisitor : public NodeVisitor {
  private:
    float radius_;
    std::unique_ptr<sf::Shape> shape_;

  public:
    explicit ShapeMakerVisitor(float radius);

    void visit(Metro& node) override;
    void visit(Home& node) override;
    void visit(Workplace& node) override;
    void visit(Entertainment& node) override;
    void visit(Node& node) override;

    sf::Shape& getShape();
};

}  // namespace zpr

#endif
