#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "city/city.h"
#include "agents/agents.h"
#include "agents/peoples.h"

class CityNetworkTest : public ::testing::Test {
protected:
    void SetUp() override {
        metro1 = std::make_shared<zpr::Metro>(1, "Metro Station 1");
        metro2 = std::make_shared<zpr::Metro>(2, "Metro Station 2");

        home1 = std::make_shared<zpr::Home>(3, "Home 1");
        home2 = std::make_shared<zpr::Home>(4, "Home 2");
        workplace1 = std::make_shared<zpr::Workplace>(5, "Office Building", 9, 17);
        workplace2 = std::make_shared<zpr::Workplace>(6, "Factory", 8, 16);
        entertainment = std::make_shared<zpr::Entertainment>(7, "Cinema");

        agent1 = std::make_shared<zpr::Agent>(1, 25);
        agent2 = std::make_shared<zpr::Agent>(2, 45);
        agent3 = std::make_shared<zpr::Agent>(3, 65);

        worker1 = std::make_shared<zpr::Worker>(10, 30, metro1, home1, workplace1);
        worker2 = std::make_shared<zpr::Worker>(11, 35, home2, home2, workplace2);
    }

    std::shared_ptr<zpr::Metro> metro1, metro2;
    std::shared_ptr<zpr::Home> home1, home2;
    std::shared_ptr<zpr::Workplace> workplace1, workplace2;
    std::shared_ptr<zpr::Entertainment> entertainment;
    std::shared_ptr<zpr::Agent> agent1, agent2, agent3;
    std::shared_ptr<zpr::Worker> worker1, worker2;
};

// ============================================================================
// Node Base Class Tests
// ==================================`==========================================
TEST_F(CityNetworkTest, NodeConstructorAndBasicGetters) {
    EXPECT_EQ(metro1->getID(), 1);
    EXPECT_EQ(metro1->getName(), "Metro Station 1");
    EXPECT_EQ(metro1->getNodeType(), "Metro");
    EXPECT_EQ(metro1->getPeopleCount(), 0);
}

TEST_F(CityNetworkTest, NodeSetters) {
    metro1->setID(100);
    metro1->setName("New Metro Name");

    EXPECT_EQ(metro1->getID(), 100);
    EXPECT_EQ(metro1->getName(), "New Metro Name");
}

TEST_F(CityNetworkTest, NodeEquality) {
    auto metro3 = std::make_shared<zpr::Metro>(1, "Different Name");
    auto metro4 = std::make_shared<zpr::Metro>(999, "Metro Station 1");

    EXPECT_TRUE(*metro1 == *metro3);
    EXPECT_FALSE(*metro1 == *metro4);
}

TEST_F(CityNetworkTest, AddAndRemovePeople) {
    EXPECT_EQ(metro1->getPeopleCount(), 0);
    EXPECT_TRUE(metro1->getPeople().empty());

    metro1->addPeople(agent1);
    metro1->addPeople(worker1);

    EXPECT_EQ(metro1->getPeopleCount(), 2);

    metro1->removePeople(agent1);
    EXPECT_EQ(metro1->getPeopleCount(), 1);

    metro1->removePeople(agent2);
    EXPECT_EQ(metro1->getPeopleCount(), 1);
}

TEST_F(CityNetworkTest, HealthStatusCounting) {
    agent1->setHealthStatus(zpr::Healthy);
    agent2->setHealthStatus(zpr::Infected);
    agent3->setHealthStatus(zpr::Sick);

    metro1->addPeople(agent1);
    metro1->addPeople(agent2);
    metro1->addPeople(agent3);

    EXPECT_EQ(metro1->getHealthyCount(), 1);
    EXPECT_EQ(metro1->getInfectedCount(), 2);
    EXPECT_EQ(metro1->getPeopleCount(), 3);
}

TEST_F(CityNetworkTest, NodeTypeIdentification) {
    EXPECT_EQ(metro1->getNodeType(), "Metro");
    EXPECT_EQ(home1->getNodeType(), "Home");
    EXPECT_EQ(workplace1->getNodeType(), "Workplace");
    EXPECT_EQ(entertainment->getNodeType(), "Entertainment");
}

// ============================================================================
// Metro Class Tests
// ============================================================================
TEST_F(CityNetworkTest, MetroConnections) {
    EXPECT_TRUE(metro1->getConnectedNodes().empty());
    EXPECT_TRUE(metro1->getAllNeighbours().empty());

    metro1->addConnection(metro2);
    metro1->addConnection(home1);

    auto connections = metro1->getConnectedNodes();
    EXPECT_EQ(connections.size(), 2);

    auto neighbours = metro1->getAllNeighbours();
    EXPECT_EQ(neighbours.size(), 2);
}

// ============================================================================
// Place Class Tests
// ============================================================================
TEST_F(CityNetworkTest, PlaceConnectingStation) {
    EXPECT_TRUE(home1->getConnecingStation().expired());

    home1->addConnectingStation(metro1);

    auto station = home1->getConnecingStation().lock();
    EXPECT_TRUE(station != nullptr);
    EXPECT_EQ(station->getID(), metro1->getID());

    auto neighbours = home1->getAllNeighbours();
    EXPECT_EQ(neighbours.size(), 1);
}

// ============================================================================
// Workplace Class Tests
// ============================================================================
TEST_F(CityNetworkTest, WorkplaceHours) {
    EXPECT_EQ(workplace1->getOpeningHour(), 9);
    EXPECT_EQ(workplace1->getClosingHour(), 17);

    workplace1->setOpeningHour(8);
    workplace1->setClosingHour(18);

    EXPECT_EQ(workplace1->getOpeningHour(), 8);
    EXPECT_EQ(workplace1->getClosingHour(), 18);
}

// ============================================================================
// Home Class Tests
// ============================================================================
TEST_F(CityNetworkTest, HomePeopleLiving) {
    EXPECT_TRUE(home1->getPeopleLivingHere().empty());

    home1->addPeopleLivingHere(worker1);
    home1->addPeopleLivingHere(agent1);

    auto residents = home1->getPeopleLivingHere();
    EXPECT_EQ(residents.size(), 2);
}

// ============================================================================
// Integration Tests
// ============================================================================
TEST_F(CityNetworkTest, NetworkConnectivity) {
    home1->addConnectingStation(metro1);
    metro1->addConnection(metro2);
    workplace1->addConnectingStation(metro2);

    auto homeNeighbours = home1->getAllNeighbours();
    EXPECT_EQ(homeNeighbours.size(), 1);

    auto metro1FromHome = homeNeighbours[0].lock();
    EXPECT_EQ(metro1FromHome->getID(), metro1->getID());

    auto metro1Neighbours = metro1->getAllNeighbours();
    EXPECT_EQ(metro1Neighbours.size(), 1);

    auto metro2FromMetro1 = metro1Neighbours[0].lock();
    EXPECT_EQ(metro2FromMetro1->getID(), metro2->getID());
}

TEST_F(CityNetworkTest, WorkerDailyCommute) {
    home1->addConnectingStation(metro1);
    metro1->addConnection(metro2);
    workplace1->addConnectingStation(metro2);

    worker1->setPosition(home1);
    home1->addPeople(worker1);
    home1->addPeopleLivingHere(worker1);

    EXPECT_EQ(home1->getPeopleCount(), 1);
    EXPECT_EQ(metro1->getPeopleCount(), 0);
    EXPECT_EQ(workplace1->getPeopleCount(), 0);

    home1->removePeople(worker1);
    worker1->setPosition(metro1);
    metro1->addPeople(worker1);
    worker1->setTarget(zpr::WorkplaceTarget);

    EXPECT_EQ(home1->getPeopleCount(), 0);
    EXPECT_EQ(metro1->getPeopleCount(), 1);
    EXPECT_EQ(worker1->getTarget(), zpr::WorkplaceTarget);

    metro1->removePeople(worker1);
    worker1->setPosition(metro2);
    metro2->addPeople(worker1);

    EXPECT_EQ(metro1->getPeopleCount(), 0);
    EXPECT_EQ(metro2->getPeopleCount(), 1);

    metro2->removePeople(worker1);
    worker1->setPosition(workplace1);
    workplace1->addPeople(worker1);

    EXPECT_EQ(metro2->getPeopleCount(), 0);
    EXPECT_EQ(workplace1->getPeopleCount(), 1);

    EXPECT_EQ(home1->getPeopleLivingHere().size(), 1);

    auto workerHome = worker1->getHome().lock();
    auto workerWorkplace = worker1->getWorkplace().lock();
    EXPECT_EQ(workerHome->getID(), home1->getID());
    EXPECT_EQ(workerWorkplace->getID(), workplace1->getID());
}

TEST_F(CityNetworkTest, InfectionSpreadSimulation) {
    agent1->setHealthStatus(zpr::Infected);
    agent2->setHealthStatus(zpr::Healthy);
    worker1->setHealthStatus(zpr::Healthy);

    metro1->addPeople(agent1);
    metro1->addPeople(agent2);
    metro1->addPeople(worker1);

    EXPECT_EQ(metro1->getInfectedCount(), 1);
    EXPECT_EQ(metro1->getHealthyCount(), 2);

    agent2->setHealthStatus(zpr::Infected);

    EXPECT_EQ(metro1->getInfectedCount(), 2);
    EXPECT_EQ(metro1->getHealthyCount(), 1);

    agent1->setHealthStatus(zpr::Sick);

    EXPECT_EQ(metro1->getInfectedCount(), 2);
    EXPECT_EQ(metro1->getHealthyCount(), 1);
}

// ============================================================================
// Edge Cases and Error Handling
// ============================================================================
TEST_F(CityNetworkTest, WeakPtrSafety) {
    {
        auto tempAgent = std::make_shared<zpr::Agent>(999, 40);
        metro1->addPeople(tempAgent);
        EXPECT_EQ(metro1->getPeopleCount(), 1);
    }

    metro1->addPeople(agent1);
    EXPECT_GE(metro1->getPeopleCount(), 1);
}

TEST_F(CityNetworkTest, WorkerWithExpiredReferences) {
    std::shared_ptr<zpr::Worker> testWorker;

    {
        auto tempHome = std::make_shared<zpr::Home>(100, "Temp Home");
        auto tempWorkplace = std::make_shared<zpr::Workplace>(101, "Temp Work", 9, 17);
        testWorker = std::make_shared<zpr::Worker>(50, 25, metro1, tempHome, tempWorkplace);

        EXPECT_FALSE(testWorker->getHome().expired());
        EXPECT_FALSE(testWorker->getWorkplace().expired());
    }

    EXPECT_TRUE(testWorker->getHome().expired());
    EXPECT_TRUE(testWorker->getWorkplace().expired());

    EXPECT_EQ(testWorker->getID(), 50);
    EXPECT_EQ(testWorker->getAge(), 25);
}

TEST_F(CityNetworkTest, MultipleAgentsVariousAges) {
    auto youngAgent = std::make_shared<zpr::Agent>(20, 15);
    auto middleAgedAgent = std::make_shared<zpr::Agent>(21, 45);
    auto elderlyAgent = std::make_shared<zpr::Agent>(22, 75);

    metro1->addPeople(youngAgent);
    metro1->addPeople(middleAgedAgent);
    metro1->addPeople(elderlyAgent);

    EXPECT_EQ(metro1->getPeopleCount(), 3);

    youngAgent->setAge(16);
    middleAgedAgent->setAge(46);
    elderlyAgent->setAge(76);

    EXPECT_EQ(youngAgent->getAge(), 16);
    EXPECT_EQ(middleAgedAgent->getAge(), 46);
    EXPECT_EQ(elderlyAgent->getAge(), 76);
}

TEST_F(CityNetworkTest, ComplexNetworkScenario) {
    auto centralMetro = std::make_shared<zpr::Metro>(100, "Central Station");
    auto residentialArea = std::make_shared<zpr::Home>(101, "Residential Complex");
    auto businessDistrict = std::make_shared<zpr::Workplace>(102, "Business District", 8, 18);
    auto shoppingCenter = std::make_shared<zpr::Entertainment>(103, "Shopping Center");

    residentialArea->addConnectingStation(centralMetro);
    businessDistrict->addConnectingStation(centralMetro);
    shoppingCenter->addConnectingStation(centralMetro);

    centralMetro->addConnection(metro1);
    centralMetro->addConnection(metro2);

    auto resident1 = std::make_shared<zpr::Worker>(200, 28, residentialArea, residentialArea, businessDistrict);
    auto resident2 = std::make_shared<zpr::Worker>(201, 32, residentialArea, residentialArea, businessDistrict);

    residentialArea->addPeopleLivingHere(resident1);
    residentialArea->addPeopleLivingHere(resident2);
    residentialArea->addPeople(resident1);
    residentialArea->addPeople(resident2);

    EXPECT_EQ(residentialArea->getPeopleLivingHere().size(), 2);
    EXPECT_EQ(residentialArea->getPeopleCount(), 2);
    EXPECT_EQ(businessDistrict->getPeopleCount(), 0);

    auto resNeighbours = residentialArea->getAllNeighbours();
    EXPECT_EQ(resNeighbours.size(), 1);

    auto centralFromRes = resNeighbours[0].lock();
    EXPECT_EQ(centralFromRes->getID(), centralMetro->getID());
}

// Performance Test
TEST_F(CityNetworkTest, LargeScaleSimulation) {
    const int numAgents = 100;
    std::vector<std::shared_ptr<zpr::Agent>> agents;

    for (int i = 0; i < numAgents; ++i) {
        auto agent = std::make_shared<zpr::Agent>(i, 20 + (i % 50));

        if (i % 10 == 0) agent->setHealthStatus(zpr::Infected);
        else if (i % 15 == 0) agent->setHealthStatus(zpr::Sick);
        else if (i % 100 == 0) agent->setHealthStatus(zpr::Dead);
        else agent->setHealthStatus(zpr::Healthy);

        agents.push_back(agent);
        metro1->addPeople(agent);
    }

    EXPECT_EQ(metro1->getPeopleCount(), numAgents);
    EXPECT_GT(metro1->getHealthyCount(), 0);
    EXPECT_GT(metro1->getInfectedCount(), 0);

    for (auto& agent : agents) {
        metro1->removePeople(agent);
    }

    EXPECT_EQ(metro1->getPeopleCount(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
