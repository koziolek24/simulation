#include <disease/disease.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace zpr {

std::string Symptom::getSymptomName()
{
    return this->symptomName_;
}

double Symptom::getSeverity()
{
    return this->severity_;
}

unsigned int Symptom::getID()
{
    return this->id_;
}

void Symptom::setSymptomName(std::string newSymptomName)
{
    this->symptomName_ = newSymptomName;
}

void Symptom::setSeverity(double newSeverity)
{
    this->severity_ = newSeverity;
}

void Symptom::setID(unsigned int newID)
{
    this->id_ = newID;
}

void Virus::addSymptom(std::shared_ptr<Symptom> newSymptom)
{
    if (this->symptomsName_.find(newSymptom->getSymptomName()) == this->symptomsName_.end()) {
        this->symptoms_.push_back(newSymptom);
        this->symptomsName_.insert(newSymptom->getSymptomName());
        this->addSeverity(newSymptom->getSeverity());
    }
}

std::vector<std::shared_ptr<Symptom>> Virus::getSymptoms()
{
    return symptoms_;
}

void Virus::addSeverity(double newSeverity)
{
    this->severity_ += newSeverity;
}

double Virus::getSeverity()
{
    return this->severity_;
}

void VirusManager::addContagiousness(double newContagiousness)
{
    this->contagiousness_.push_back(newContagiousness);
}

void VirusManager::addMortality(double newMortality)
{
    this->mortality_.push_back(newMortality);
}

void VirusManager::addSymptom(std::shared_ptr<Symptom> newSymptom)
{
    this->symptoms_.push_back(newSymptom);
}

std::vector<double> VirusManager::getMortality()
{
    return this->mortality_;
}

std::vector<double> VirusManager::getContagiousness()
{
    return this->contagiousness_;
}

std::vector<std::shared_ptr<Symptom>> VirusManager::getSymptoms()
{
    return this->symptoms_;
}

std::shared_ptr<Symptom> VirusManager::getSymptomByName(std::string name)
{
    for (auto symptom : this->symptoms_) {
        if (symptom->getSymptomName() == name) {
            return symptom;
        }
    }
    return nullptr;
}

unsigned int Virus::getSymptomsID()
{
    unsigned int id = 0;
    for (auto symptom : this->getSymptoms()) {
        id += symptom->getID();
    }
    return id;
}

double VirusManager::getMortalityByID(unsigned int id)
{
    return this->mortality_[id];
}

double VirusManager::getContagiousnessByID(unsigned int id)
{
    return this->contagiousness_[id];
}

double Symptom::getEvolveProbability(unsigned int id)
{
    return this->evolveProbability_[id];
}

void Symptom::addEvolveProbability(double probability)
{
    this->evolveProbability_.push_back(probability);
}

std::vector<std::string> Symptom::getDependable()
{
    return this->dependable_;
}

double Virus::getEvolveSymptomProbability(std::shared_ptr<Symptom> symptom)
{
    unsigned int probabilityID = 0;
    unsigned int dependableID = 1;
    for (auto dependable : symptom->getDependable()) {
        if (this->symptomsName_.find(dependable) != this->symptomsName_.end()) {
            probabilityID += dependableID;
        }
        dependableID *= 2;
    }
    return symptom->getEvolveProbability(probabilityID);
}

void Virus::evolve(std::vector<std::shared_ptr<Symptom>> allSymptoms)
{
    for (auto symptom : allSymptoms) {
        std::string name = symptom->getSymptomName();
        if (this->symptomsName_.find(name) == this->symptomsName_.end()) {
            double probability = this->getEvolveSymptomProbability(symptom);
            if (probability >= ((double)rand()) / RAND_MAX) {
                this->addSymptom(symptom);
            }
        }
    }
}

void Symptom::addDependable(std::string dependable)
{
    this->dependable_.push_back(dependable);
}

std::shared_ptr<Symptom> VirusManager::getInitialSymptom()
{
    unsigned long size = this->initialSymptoms_.size();
    return this->initialSymptoms_[(unsigned long)rand() % size];
}

void VirusManager::addInitialSymptom(std::shared_ptr<Symptom> symptom)
{
    this->initialSymptoms_.push_back(symptom);
}
}  // namespace zpr