#include <gtest/gtest.h>

#include "agents/agents.h"

using namespace zpr;

TEST(AgentTest, AgeIsSetCorrectly)
{
    Agent agent(0, 25);
    EXPECT_EQ(agent.getAge(), 25);
}

TEST(AgentTest, HealthStatusDefaultsToHealthy)
{
    Agent agent(0, 30);
    EXPECT_EQ(agent.getHealthStatus(), "Healthy");
}

TEST(AgentTest, CanChangeHealthStatus)
{
    Agent agent(0, 40);
    agent.setHealthStatus(Infected);
    EXPECT_EQ(agent.getHealthStatus(), "Infected");
}
