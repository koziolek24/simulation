#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "disease/disease.h"

using namespace zpr;

class SymptomTest : public ::testing::Test {
protected:
    void SetUp() override {
        symptom = std::make_shared<Symptom>(1, "Fever", 5.5);
    }
    
    std::shared_ptr<Symptom> symptom;
};

TEST_F(SymptomTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(symptom->getID(), 1);
    EXPECT_EQ(symptom->getSymptomName(), "Fever");
    EXPECT_DOUBLE_EQ(symptom->getSeverity(), 5.5);
    EXPECT_TRUE(symptom->getDependable().empty());
}

TEST_F(SymptomTest, SettersAndGettersWork) {
    symptom->setSymptomName("Cough");
    symptom->setSeverity(3.2);
    symptom->setID(42);
    
    EXPECT_EQ(symptom->getSymptomName(), "Cough");
    EXPECT_DOUBLE_EQ(symptom->getSeverity(), 3.2);
    EXPECT_EQ(symptom->getID(), 42);
}

TEST_F(SymptomTest, AddDependableWorks) {
    symptom->addDependable("Headache");
    symptom->addDependable("Fatigue");
    
    auto dependable = symptom->getDependable();
    EXPECT_EQ(dependable.size(), 2);
    EXPECT_EQ(dependable[0], "Headache");
    EXPECT_EQ(dependable[1], "Fatigue");
}

TEST_F(SymptomTest, EvolveProbabilityWorks) {
    symptom->addEvolveProbability(0.1);
    symptom->addEvolveProbability(0.3);
    symptom->addEvolveProbability(0.7);
    
    EXPECT_DOUBLE_EQ(symptom->getEvolveProbability(0), 0.1);
    EXPECT_DOUBLE_EQ(symptom->getEvolveProbability(1), 0.3);
    EXPECT_DOUBLE_EQ(symptom->getEvolveProbability(2), 0.7);
}

class VirusTest : public ::testing::Test {
protected:
    void SetUp() override {
        virus = std::make_unique<Virus>();
        fever = std::make_shared<Symptom>(1, "Fever", 5.0);
        cough = std::make_shared<Symptom>(2, "Cough", 3.0);
        headache = std::make_shared<Symptom>(3, "Headache", 2.0);
    }
    
    std::unique_ptr<Virus> virus;
    std::shared_ptr<Symptom> fever;
    std::shared_ptr<Symptom> cough;
    std::shared_ptr<Symptom> headache;
};

TEST_F(VirusTest, InitialStateIsEmpty) {
    EXPECT_EQ(virus->getSymptoms().size(), 0);
    EXPECT_DOUBLE_EQ(virus->getSeverity(), 0.0);
    EXPECT_EQ(virus->getSymptomsID(), 0);
}

TEST_F(VirusTest, AddSymptomWorks) {
    virus->addSymptom(fever);
    virus->addSymptom(cough);
    
    auto symptoms = virus->getSymptoms();
    EXPECT_EQ(symptoms.size(), 2);
    EXPECT_DOUBLE_EQ(virus->getSeverity(), 8.0);
    EXPECT_EQ(virus->getSymptomsID(), 3);
}

TEST_F(VirusTest, DuplicateSymptomNotAdded) {
    virus->addSymptom(fever);
    virus->addSymptom(fever);
    
    EXPECT_EQ(virus->getSymptoms().size(), 1);
    EXPECT_DOUBLE_EQ(virus->getSeverity(), 5.0);
}

TEST_F(VirusTest, EvolveSymptomProbabilityCalculation) {
    fever->addDependable("Headache");
    fever->addEvolveProbability(0.1);
    fever->addEvolveProbability(0.8);

    virus->addSymptom(headache);
    
    double probability = virus->getEvolveSymptomProbability(fever);
    EXPECT_DOUBLE_EQ(probability, 0.8);
}

TEST_F(VirusTest, EvolveAddsNewSymptoms) {
    fever->addDependable("Headache");
    fever->addEvolveProbability(0.0);
    cough->addEvolveProbability(0.0);
    headache->addEvolveProbability(0.0);
    fever->addEvolveProbability(1.0);
    
    virus->addSymptom(headache);
    
    std::vector<std::shared_ptr<Symptom>> allSymptoms = {fever, cough, headache};
    EXPECT_EQ(fever->getDependable().size(), 1);
    EXPECT_EQ(virus->getSymptoms().size(), 1);
    virus->evolve(allSymptoms);
    
    EXPECT_EQ(virus->getSymptoms().size(), 2);
}

class VirusManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = std::make_unique<VirusManager>();
        fever = std::make_shared<Symptom>(0, "Fever", 5.0);
        cough = std::make_shared<Symptom>(1, "Cough", 3.0);
        headache = std::make_shared<Symptom>(2, "Headache", 2.0);
    }
    
    std::unique_ptr<VirusManager> manager;
    std::shared_ptr<Symptom> fever;
    std::shared_ptr<Symptom> cough;
    std::shared_ptr<Symptom> headache;
};

TEST_F(VirusManagerTest, InitialStateIsEmpty) {
    EXPECT_TRUE(manager->getSymptoms().empty());
    EXPECT_TRUE(manager->getMortality().empty());
    EXPECT_TRUE(manager->getContagiousness().empty());
}

TEST_F(VirusManagerTest, AddSymptomWorks) {
    manager->addSymptom(fever);
    manager->addSymptom(cough);
    
    auto symptoms = manager->getSymptoms();
    EXPECT_EQ(symptoms.size(), 2);
    EXPECT_EQ(symptoms[0]->getSymptomName(), "Fever");
    EXPECT_EQ(symptoms[1]->getSymptomName(), "Cough");
}

TEST_F(VirusManagerTest, GetSymptomByNameWorks) {
    manager->addSymptom(fever);
    manager->addSymptom(cough);
    
    auto foundSymptom = manager->getSymptomByName("Fever");
    EXPECT_NE(foundSymptom, nullptr);
    EXPECT_EQ(foundSymptom->getSymptomName(), "Fever");
    
    auto notFound = manager->getSymptomByName("NonExistent");
    EXPECT_EQ(notFound, nullptr);
}

TEST_F(VirusManagerTest, MortalityManagement) {
    manager->addMortality(0.1);
    manager->addMortality(0.05);
    manager->addMortality(0.15);
    
    auto mortality = manager->getMortality();
    EXPECT_EQ(mortality.size(), 3);
    EXPECT_DOUBLE_EQ(manager->getMortalityByID(0), 0.1);
    EXPECT_DOUBLE_EQ(manager->getMortalityByID(1), 0.05);
    EXPECT_DOUBLE_EQ(manager->getMortalityByID(2), 0.15);
}

TEST_F(VirusManagerTest, ContagiousnessManagement) {
    manager->addContagiousness(0.8);
    manager->addContagiousness(0.6);
    manager->addContagiousness(0.9);
    
    auto contagiousness = manager->getContagiousness();
    EXPECT_EQ(contagiousness.size(), 3);
    EXPECT_DOUBLE_EQ(manager->getContagiousnessByID(0), 0.8);
    EXPECT_DOUBLE_EQ(manager->getContagiousnessByID(1), 0.6);
    EXPECT_DOUBLE_EQ(manager->getContagiousnessByID(2), 0.9);
}

TEST_F(VirusManagerTest, InitialSymptomManagement) {
    manager->addInitialSymptom(fever);
    manager->addInitialSymptom(cough);
    
    auto initialSymptom = manager->getInitialSymptom();
    EXPECT_NE(initialSymptom, nullptr);
    EXPECT_TRUE(initialSymptom == fever || initialSymptom == cough);
}