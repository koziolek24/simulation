#ifndef NODEVISITOR_H
#define NODEVISITOR_H

namespace zpr {
class Node;
class Metro;
class Home;
class Workplace;
class Entertainment;

class NodeVisitor {
  public:
    virtual void visit(Metro& node) = 0;
    virtual void visit(Home& node) = 0;
    virtual void visit(Workplace& node) = 0;
    virtual void visit(Entertainment& node) = 0;
    virtual void visit(Node& node) = 0;
    virtual ~NodeVisitor() = default;
};
}  // namespace zpr

#endif
