#include <city/city.h>
#include <gtest/gtest.h>

#include <memory>

#include "agents/peoples.h"

using namespace zpr;

TEST(WorkerTest, HomeIsSetProperly)
{
    Home home = Home(0, "dom1");
    Workplace workplace = Workplace(1, "workplace1", 6, 18);
    std::shared_ptr<zpr::Home> sHome = std::make_shared<zpr::Home>(home);
    std::shared_ptr<zpr::Workplace> sWorkplace = std::make_shared<zpr::Workplace>(workplace);
    Worker worker = Worker(0, 18, sHome, sHome, sWorkplace);
    std::shared_ptr<Worker> sWorker = std::make_shared<Worker>(worker);
    Home home2 = Home(2, "home2");
    std::shared_ptr<Home> sHome2 = std::make_shared<Home>(home2);
    sWorker->setHome(sHome2);
    sHome->addPeopleLivingHere(sWorker);
    EXPECT_EQ(sWorker->getHome().lock()->getName(), "home2");
}