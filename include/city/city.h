// Maciej Kozlowski
/*
 * Plik zawiera Encje do sieci miasta
 * Klasa podstawowa to Node zawierajaca wektor weak_ptr ludzi obecnych w danym momencie oraz nazwe,
 * pozwala ona liczyc ilosc osob zarazonych, oraz zdrowych Metro to encja pozwalajaca sie poruszac
 * miedzy roznymi miejscami, zawiera ona wektor polaczonych ze soba punktow Kolejna podstawowa klasa
 * to Place, ktora zawiera wskaznik do stacji metra z ktora jest polaczona
 *
 */

#ifndef CITY_H
#define CITY_H
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "agents/agents.h"
#include "visitors/NodeVisitor.h"
namespace zpr {

enum nodeType { nodeType, metroType, homeType, workplaceType, entertainmentType };
class Node {
  private:
    unsigned int id_;
    std::vector<std::weak_ptr<zpr::Agent>> presentPeople_;
    std::string name_;
    enum nodeType type_;
    std::mutex nodeMutex_;

  public:
    Node(unsigned int id, std::string newName, enum nodeType type = nodeType)
        : id_(id), presentPeople_(), name_(newName), type_(type)
    {
    }
    virtual ~Node() = default;
    const std::vector<std::weak_ptr<zpr::Agent>> getPeople();
    void addPeople(const std::shared_ptr<zpr::Agent>& newAgent);
    void removePeople(const std::shared_ptr<zpr::Agent>& agentToRemove);
    std::string getName();
    unsigned int getInfectedCount();
    unsigned int getHealthyCount();
    unsigned int getPeopleCount();
    unsigned int getID();
    enum nodeType getNodeType();
    std::mutex& getMutex();

    void setID(unsigned int newID);
    void setName(std::string newName);

    virtual const std::vector<std::weak_ptr<zpr::Node>> getAllNeighbours() = 0;

    sf::Vector2f pos_{0.f, 0.f};
    sf::Vector2f vel_{0.f, 0.f};

    virtual void accept(NodeVisitor& visitor) = 0;

    bool operator==(Node& node);
};
class Metro : public Node {
  private:
    std::vector<std::weak_ptr<zpr::Node>> connectedNodes_;

  public:
    Metro(unsigned int id, std::string newName, enum nodeType type = metroType)
        : Node(id, newName, type), connectedNodes_()
    {
    }
    const std::vector<std::weak_ptr<zpr::Node>> getConnectedNodes();
    void addConnection(const std::shared_ptr<zpr::Node>& newNode);
    const std::vector<std::weak_ptr<zpr::Node>> getAllNeighbours() override;

    void accept(NodeVisitor& visitor) override;
};

class Place : public Node {
  private:
    std::weak_ptr<zpr::Metro> connectingStation_;

  public:
    Place(unsigned int id, std::string newName, enum nodeType type)
        : Node(id, newName, type), connectingStation_()
    {
    }
    void addConnectingStation(const std::shared_ptr<zpr::Metro>& connectingStation);
    const std::weak_ptr<zpr::Metro> getConnecingStation();
    const std::vector<std::weak_ptr<zpr::Node>> getAllNeighbours() override;

    void accept(NodeVisitor& visitor) override;
};

class Workplace : public Place {
  private:
    unsigned int openingHour_;
    unsigned int closingHour_;

  public:
    Workplace(unsigned int id, std::string newName, unsigned int openingHour,
              unsigned int closingHour, enum nodeType type = workplaceType)
        : Place(id, newName, type), openingHour_(openingHour), closingHour_(closingHour)
    {
    }
    unsigned int getOpeningHour();
    unsigned int getClosingHour();
    void setOpeningHour(unsigned int newOpeningHour);
    void setClosingHour(unsigned int newClosingHour);

    void accept(NodeVisitor& visitor) override;
};
class Home : public Place {
  private:
    std::vector<std::weak_ptr<zpr::Agent>> peopleLivingHere_;

  public:
    Home(unsigned int id, std::string newName, enum nodeType type = homeType)
        : Place(id, newName, type)
    {
    }
    void addPeopleLivingHere(std::shared_ptr<zpr::Agent> newPeople);
    const std::vector<std::weak_ptr<zpr::Agent>> getPeopleLivingHere();

    void accept(NodeVisitor& visitor) override;
};

class Entertainment : public Place {
  public:
    Entertainment(unsigned int id, std::string newName, enum nodeType type = entertainmentType)
        : Place(id, newName, type)
    {
    }

    void accept(NodeVisitor& visitor) override;
};
}  // namespace zpr
#endif
