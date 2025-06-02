#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "agents/agents.h"
#include "agents/peoples.h"
#include "city/city.h"

class AgentTest : public ::testing::Test {
  protected:
    void SetUp() override
    {
        agent1 = std::make_shared<zpr::Agent>(1, 25);
        agent2 = std::make_shared<zpr::Agent>(2, 45);
        agent3 = std::make_shared<zpr::Agent>(3, 65);
    }

    std::shared_ptr<zpr::Agent> agent1, agent2, agent3;
};

TEST_F(AgentTest, DefaultConstructor)
{
    auto agent = std::make_shared<zpr::Agent>(100, 30);

    EXPECT_EQ(agent->getID(), 100);
    EXPECT_EQ(agent->getAge(), 30);
    EXPECT_EQ(agent->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(agent->getTarget(), zpr::HomeTarget);
}

TEST_F(AgentTest, ConstructorWithDifferentAges)
{
    auto child = std::make_shared<zpr::Agent>(1, 5);
    auto teenager = std::make_shared<zpr::Agent>(2, 16);
    auto adult = std::make_shared<zpr::Agent>(3, 35);
    auto senior = std::make_shared<zpr::Agent>(4, 75);

    EXPECT_EQ(child->getAge(), 5);
    EXPECT_EQ(teenager->getAge(), 16);
    EXPECT_EQ(adult->getAge(), 35);
    EXPECT_EQ(senior->getAge(), 75);

    EXPECT_EQ(child->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(teenager->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(adult->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(senior->getHealthStatus(), zpr::Healthy);

    EXPECT_EQ(child->getTarget(), zpr::HomeTarget);
    EXPECT_EQ(teenager->getTarget(), zpr::HomeTarget);
    EXPECT_EQ(adult->getTarget(), zpr::HomeTarget);
    EXPECT_EQ(senior->getTarget(), zpr::HomeTarget);
}

TEST_F(AgentTest, IDGetterAndSetter)
{
    EXPECT_EQ(agent1->getID(), 1);

    agent1->setID(999);
    EXPECT_EQ(agent1->getID(), 999);

    agent1->setID(0);
    EXPECT_EQ(agent1->getID(), 0);

    agent1->setID(UINT_MAX);
    EXPECT_EQ(agent1->getID(), UINT_MAX);
}

TEST_F(AgentTest, AgeGetterAndSetter)
{
    EXPECT_EQ(agent1->getAge(), 25);

    agent1->setAge(50);
    EXPECT_EQ(agent1->getAge(), 50);

    agent1->setAge(0);
    EXPECT_EQ(agent1->getAge(), 0);

    agent1->setAge(120);
    EXPECT_EQ(agent1->getAge(), 120);
}

TEST_F(AgentTest, HealthStatusGetterAndSetter)
{
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Healthy);

    agent1->setHealthStatus(zpr::Infected);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Infected);

    agent1->setHealthStatus(zpr::Sick);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Sick);

    agent1->setHealthStatus(zpr::Dead);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Dead);

    agent1->setHealthStatus(zpr::Healthy);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Healthy);
}

TEST_F(AgentTest, TargetGetterAndSetter)
{
    EXPECT_EQ(agent1->getTarget(), zpr::HomeTarget);

    agent1->setTarget(zpr::WorkplaceTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::WorkplaceTarget);

    agent1->setTarget(zpr::EntertainmentTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::EntertainmentTarget);

    agent1->setTarget(zpr::HomeTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::HomeTarget);
}

TEST_F(AgentTest, MultipleAgentsIndependence)
{
    agent1->setID(100);
    agent1->setAge(30);
    agent1->setHealthStatus(zpr::Infected);
    agent1->setTarget(zpr::WorkplaceTarget);

    EXPECT_EQ(agent2->getID(), 2);
    EXPECT_EQ(agent2->getAge(), 45);
    EXPECT_EQ(agent2->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(agent2->getTarget(), zpr::HomeTarget);

    EXPECT_EQ(agent3->getID(), 3);
    EXPECT_EQ(agent3->getAge(), 65);
    EXPECT_EQ(agent3->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(agent3->getTarget(), zpr::HomeTarget);
}

TEST_F(AgentTest, HealthStatusProgression)
{
    agent1->setHealthStatus(zpr::Healthy);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Healthy);

    agent1->setHealthStatus(zpr::Infected);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Infected);

    agent1->setHealthStatus(zpr::Sick);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Sick);

    agent1->setHealthStatus(zpr::Healthy);
    EXPECT_EQ(agent1->getHealthStatus(), zpr::Healthy);

    agent2->setHealthStatus(zpr::Infected);
    agent2->setHealthStatus(zpr::Sick);
    agent2->setHealthStatus(zpr::Dead);
    EXPECT_EQ(agent2->getHealthStatus(), zpr::Dead);
}

TEST_F(AgentTest, DailyRoutineTargets)
{
    agent1->setTarget(zpr::WorkplaceTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::WorkplaceTarget);

    agent1->setTarget(zpr::EntertainmentTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::EntertainmentTarget);

    agent1->setTarget(zpr::HomeTarget);
    EXPECT_EQ(agent1->getTarget(), zpr::HomeTarget);
}

// ============================================================================
// Worker Class Unit Tests
// ============================================================================
class WorkerTest : public ::testing::Test {
  protected:
    void SetUp() override
    {
        metro1 = std::make_shared<zpr::Metro>(1, "Metro Station 1");
        metro2 = std::make_shared<zpr::Metro>(2, "Metro Station 2");
        home1 = std::make_shared<zpr::Home>(3, "Home 1");
        home2 = std::make_shared<zpr::Home>(4, "Home 2");
        workplace1 = std::make_shared<zpr::Workplace>(5, "Office 1", 9, 17);
        workplace2 = std::make_shared<zpr::Workplace>(6, "Factory", 8, 16);

        worker1 = std::make_shared<zpr::Worker>(10, 30, metro1, home1, workplace1);
        worker2 = std::make_shared<zpr::Worker>(11, 35, home2, home2, workplace2);
    }

    std::shared_ptr<zpr::Metro> metro1, metro2;
    std::shared_ptr<zpr::Home> home1, home2;
    std::shared_ptr<zpr::Workplace> workplace1, workplace2;
    std::shared_ptr<zpr::Worker> worker1, worker2;
};

TEST_F(WorkerTest, ConstructorAndInheritance)
{
    EXPECT_EQ(worker1->getID(), 10);
    EXPECT_EQ(worker1->getAge(), 30);
    EXPECT_EQ(worker1->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(worker1->getTarget(), zpr::HomeTarget);

    auto home = worker1->getHome().lock();
    auto workplace = worker1->getWorkplace().lock();
    auto position = worker1->getPosition().lock();

    ASSERT_TRUE(home != nullptr);
    ASSERT_TRUE(workplace != nullptr);
    ASSERT_TRUE(position != nullptr);

    EXPECT_EQ(home->getID(), home1->getID());
    EXPECT_EQ(workplace->getID(), workplace1->getID());
    EXPECT_EQ(position->getID(), metro1->getID());
}

TEST_F(WorkerTest, InheritedAgentFunctionality)
{
    worker1->setID(100);
    worker1->setAge(40);
    worker1->setHealthStatus(zpr::Infected);
    worker1->setTarget(zpr::WorkplaceTarget);

    EXPECT_EQ(worker1->getID(), 100);
    EXPECT_EQ(worker1->getAge(), 40);
    EXPECT_EQ(worker1->getHealthStatus(), zpr::Infected);
    EXPECT_EQ(worker1->getTarget(), zpr::WorkplaceTarget);
}

TEST_F(WorkerTest, HomeGetterAndSetter)
{
    auto initialHome = worker1->getHome().lock();
    EXPECT_EQ(initialHome->getID(), home1->getID());

    worker1->setHome(home2);
    auto newHome = worker1->getHome().lock();
    EXPECT_EQ(newHome->getID(), home2->getID());
}

TEST_F(WorkerTest, WorkplaceGetterAndSetter)
{
    auto initialWorkplace = worker1->getWorkplace().lock();
    EXPECT_EQ(initialWorkplace->getID(), workplace1->getID());

    worker1->setWorkplace(workplace2);
    auto newWorkplace = worker1->getWorkplace().lock();
    EXPECT_EQ(newWorkplace->getID(), workplace2->getID());
}

TEST_F(WorkerTest, PositionGetterAndSetter)
{
    auto initialPosition = worker1->getPosition().lock();
    EXPECT_EQ(initialPosition->getID(), metro1->getID());

    worker1->setPosition(metro2);
    auto newPosition = worker1->getPosition().lock();
    EXPECT_EQ(newPosition->getID(), metro2->getID());

    worker1->setPosition(workplace1);
    auto workPosition = worker1->getPosition().lock();
    EXPECT_EQ(workPosition->getID(), workplace1->getID());
}

TEST_F(WorkerTest, MultipleWorkersIndependence)
{
    worker1->setID(200);
    worker1->setAge(50);
    worker1->setHealthStatus(zpr::Sick);
    worker1->setHome(home2);
    worker1->setWorkplace(workplace2);
    worker1->setPosition(metro2);

    EXPECT_EQ(worker2->getID(), 11);
    EXPECT_EQ(worker2->getAge(), 35);
    EXPECT_EQ(worker2->getHealthStatus(), zpr::Healthy);

    auto worker2Home = worker2->getHome().lock();
    auto worker2Workplace = worker2->getWorkplace().lock();
    auto worker2Position = worker2->getPosition().lock();

    EXPECT_EQ(worker2Home->getID(), home2->getID());
    EXPECT_EQ(worker2Workplace->getID(), workplace2->getID());
    EXPECT_EQ(worker2Position->getID(), home2->getID());
}

TEST_F(WorkerTest, WorkerLifecycleSimulation)
{
    worker1->setPosition(home1);
    worker1->setTarget(zpr::HomeTarget);

    auto currentPos = worker1->getPosition().lock();
    EXPECT_EQ(currentPos->getID(), home1->getID());
    EXPECT_EQ(worker1->getTarget(), zpr::HomeTarget);

    worker1->setTarget(zpr::WorkplaceTarget);
    worker1->setPosition(metro1);

    currentPos = worker1->getPosition().lock();
    EXPECT_EQ(currentPos->getID(), metro1->getID());
    EXPECT_EQ(worker1->getTarget(), zpr::WorkplaceTarget);

    worker1->setPosition(workplace1);

    currentPos = worker1->getPosition().lock();
    EXPECT_EQ(currentPos->getID(), workplace1->getID());

    worker1->setTarget(zpr::EntertainmentTarget);
    worker1->setPosition(metro1);

    EXPECT_EQ(worker1->getTarget(), zpr::EntertainmentTarget);

    worker1->setTarget(zpr::HomeTarget);
    worker1->setPosition(home1);

    currentPos = worker1->getPosition().lock();
    EXPECT_EQ(currentPos->getID(), home1->getID());
    EXPECT_EQ(worker1->getTarget(), zpr::HomeTarget);
}

TEST_F(WorkerTest, WeakPointerSafety)
{
    std::shared_ptr<zpr::Worker> testWorker;

    {
        auto tempHome = std::make_shared<zpr::Home>(100, "Temp Home");
        auto tempWorkplace = std::make_shared<zpr::Workplace>(101, "Temp Workplace", 9, 17);
        auto tempPosition = std::make_shared<zpr::Metro>(102, "Temp Metro");

        testWorker = std::make_shared<zpr::Worker>(50, 25, tempPosition, tempHome, tempWorkplace);

        EXPECT_FALSE(testWorker->getHome().expired());
        EXPECT_FALSE(testWorker->getWorkplace().expired());
        EXPECT_FALSE(testWorker->getPosition().expired());

        auto home = testWorker->getHome().lock();
        auto workplace = testWorker->getWorkplace().lock();
        auto position = testWorker->getPosition().lock();

        EXPECT_TRUE(home != nullptr);
        EXPECT_TRUE(workplace != nullptr);
        EXPECT_TRUE(position != nullptr);

        EXPECT_EQ(home->getID(), 100);
        EXPECT_EQ(workplace->getID(), 101);
        EXPECT_EQ(position->getID(), 102);
    }

    EXPECT_TRUE(testWorker->getHome().expired());
    EXPECT_TRUE(testWorker->getWorkplace().expired());
    EXPECT_TRUE(testWorker->getPosition().expired());

    EXPECT_EQ(testWorker->getID(), 50);
    EXPECT_EQ(testWorker->getAge(), 25);
    EXPECT_EQ(testWorker->getHealthStatus(), zpr::Healthy);

    testWorker->setHome(home1);
    testWorker->setWorkplace(workplace1);
    testWorker->setPosition(metro1);

    EXPECT_FALSE(testWorker->getHome().expired());
    EXPECT_FALSE(testWorker->getWorkplace().expired());
    EXPECT_FALSE(testWorker->getPosition().expired());
}

TEST_F(WorkerTest, JobChangeScenario)
{
    auto initialWorkplace = worker1->getWorkplace().lock();
    EXPECT_EQ(initialWorkplace->getID(), workplace1->getID());

    worker1->setWorkplace(workplace2);
    auto newWorkplace = worker1->getWorkplace().lock();
    EXPECT_EQ(newWorkplace->getID(), workplace2->getID());

    worker1->setHome(home2);
    auto newHome = worker1->getHome().lock();
    EXPECT_EQ(newHome->getID(), home2->getID());

    worker1->setPosition(home2);
    auto currentPos = worker1->getPosition().lock();
    EXPECT_EQ(currentPos->getID(), home2->getID());
}

TEST_F(WorkerTest, WorkerHealthAndMovement)
{
    EXPECT_EQ(worker1->getHealthStatus(), zpr::Healthy);
    worker1->setPosition(workplace1);
    worker1->setTarget(zpr::WorkplaceTarget);

    auto pos = worker1->getPosition().lock();
    EXPECT_EQ(pos->getID(), workplace1->getID());
    EXPECT_EQ(worker1->getTarget(), zpr::WorkplaceTarget);

    worker1->setHealthStatus(zpr::Infected);
    EXPECT_EQ(worker1->getHealthStatus(), zpr::Infected);

    pos = worker1->getPosition().lock();
    EXPECT_EQ(pos->getID(), workplace1->getID());
    EXPECT_EQ(worker1->getTarget(), zpr::WorkplaceTarget);

    worker1->setHealthStatus(zpr::Sick);
    worker1->setTarget(zpr::HomeTarget);
    worker1->setPosition(home1);

    EXPECT_EQ(worker1->getHealthStatus(), zpr::Sick);
    EXPECT_EQ(worker1->getTarget(), zpr::HomeTarget);

    pos = worker1->getPosition().lock();
    EXPECT_EQ(pos->getID(), home1->getID());
}

// ============================================================================
// Integration Tests: Agent and Worker interaction
// ============================================================================
class AgentWorkerIntegrationTest : public ::testing::Test {
  protected:
    void SetUp() override
    {
        metro = std::make_shared<zpr::Metro>(1, "Central Metro");
        home = std::make_shared<zpr::Home>(2, "Family Home");
        workplace = std::make_shared<zpr::Workplace>(3, "Office", 9, 17);

        regularAgent = std::make_shared<zpr::Agent>(10, 65);
        worker = std::make_shared<zpr::Worker>(20, 35, home, home, workplace);
        youngAgent = std::make_shared<zpr::Agent>(30, 16);
    }

    std::shared_ptr<zpr::Metro> metro;
    std::shared_ptr<zpr::Home> home;
    std::shared_ptr<zpr::Workplace> workplace;
    std::shared_ptr<zpr::Agent> regularAgent, youngAgent;
    std::shared_ptr<zpr::Worker> worker;
};

TEST_F(AgentWorkerIntegrationTest, MixedAgentTypes)
{
    regularAgent->setHealthStatus(zpr::Healthy);
    worker->setHealthStatus(zpr::Healthy);
    youngAgent->setHealthStatus(zpr::Healthy);

    EXPECT_EQ(regularAgent->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(worker->getHealthStatus(), zpr::Healthy);
    EXPECT_EQ(youngAgent->getHealthStatus(), zpr::Healthy);

    auto workerHome = worker->getHome().lock();
    auto workerWorkplace = worker->getWorkplace().lock();

    EXPECT_TRUE(workerHome != nullptr);
    EXPECT_TRUE(workerWorkplace != nullptr);
    EXPECT_EQ(workerHome->getID(), home->getID());
    EXPECT_EQ(workerWorkplace->getID(), workplace->getID());
}

TEST_F(AgentWorkerIntegrationTest, AgeBasedBehavior)
{
    EXPECT_EQ(regularAgent->getAge(), 65);
    EXPECT_EQ(worker->getAge(), 35);
    EXPECT_EQ(youngAgent->getAge(), 16);

    regularAgent->setTarget(zpr::EntertainmentTarget);
    worker->setTarget(zpr::WorkplaceTarget);
    youngAgent->setTarget(zpr::HomeTarget);

    EXPECT_EQ(regularAgent->getTarget(), zpr::EntertainmentTarget);
    EXPECT_EQ(worker->getTarget(), zpr::WorkplaceTarget);
    EXPECT_EQ(youngAgent->getTarget(), zpr::HomeTarget);
}

TEST_F(AgentWorkerIntegrationTest, InfectionSpreadScenario)
{
    regularAgent->setHealthStatus(zpr::Infected);
    worker->setHealthStatus(zpr::Healthy);
    youngAgent->setHealthStatus(zpr::Healthy);

    worker->setHealthStatus(zpr::Infected);
    youngAgent->setHealthStatus(zpr::Infected);

    EXPECT_EQ(regularAgent->getHealthStatus(), zpr::Infected);
    EXPECT_EQ(worker->getHealthStatus(), zpr::Infected);
    EXPECT_EQ(youngAgent->getHealthStatus(), zpr::Infected);

    regularAgent->setHealthStatus(zpr::Sick);
    worker->setHealthStatus(zpr::Sick);

    EXPECT_EQ(regularAgent->getHealthStatus(), zpr::Sick);
    EXPECT_EQ(worker->getHealthStatus(), zpr::Sick);
    EXPECT_EQ(youngAgent->getHealthStatus(), zpr::Infected);
}

// ============================================================================
// Edge Cases and Stress Tests
// ============================================================================
TEST_F(AgentTest, EdgeCaseIDs)
{
    auto agent = std::make_shared<zpr::Agent>(0, 25);
    EXPECT_EQ(agent->getID(), 0);

    agent->setID(UINT_MAX);
    EXPECT_EQ(agent->getID(), UINT_MAX);
}

TEST_F(AgentTest, EdgeCaseAges)
{
    auto agent = std::make_shared<zpr::Agent>(1, 0);
    EXPECT_EQ(agent->getAge(), 0);

    agent->setAge(UINT_MAX);
    EXPECT_EQ(agent->getAge(), UINT_MAX);
}

TEST_F(AgentTest, RapidStateChanges)
{
    for (unsigned int i = 0; i < 100; ++i) {
        agent1->setID(i);
        agent1->setAge(i % 100);
        agent1->setHealthStatus(static_cast<zpr::healthStatus>(i % 4));
        agent1->setTarget(static_cast<zpr::target>(i % 3));

        EXPECT_EQ(agent1->getID(), static_cast<unsigned int>(i));
        EXPECT_EQ(agent1->getAge(), static_cast<unsigned int>(i % 100));
        EXPECT_EQ(agent1->getHealthStatus(), static_cast<zpr::healthStatus>(i % 4));
        EXPECT_EQ(agent1->getTarget(), static_cast<zpr::target>(i % 3));
    }
}

TEST_F(WorkerTest, RapidLocationChanges)
{
    std::vector<std::shared_ptr<zpr::Node>> locations = {home1, metro1, workplace1, metro2, home2};

    for (int i = 0; i < 50; ++i) {
        auto newLocation = locations[(long unsigned int)i % locations.size()];
        worker1->setPosition(newLocation);

        auto currentPos = worker1->getPosition().lock();
        EXPECT_EQ(currentPos->getID(), newLocation->getID());
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
