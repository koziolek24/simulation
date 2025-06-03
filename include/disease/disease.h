#ifndef DISEASE_H
#define DISEASE_H

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace zpr {

class Symptom {
  private:
    unsigned int id_;
    std::string symptomName_;
    double severity_;
    std::vector<std::string> dependable_;
    std::vector<double> evolveProbability_;

  public:
    Symptom(unsigned int id, std::string symptomName, double severity)
        : id_(id), symptomName_(symptomName), severity_(severity), dependable_()
    {
    }
    std::string getSymptomName();
    double getSeverity();
    unsigned int getID();
    std::vector<std::string> getDependable();
    void setSymptomName(std::string newSymptomName);
    void setSeverity(double newSeverity);
    void setID(unsigned int newID);
    void addDependable(std::string newDependable);
    void addEvolveProbability(double probability);
    double getEvolveProbability(unsigned int id);
};

class Virus {
  private:
    std::vector<std::shared_ptr<Symptom>> symptoms_;
    std::set<std::string> symptomsName_;
    double severity_;

  public:
    Virus() : symptoms_(), symptomsName_(), severity_(0) {}
    void addSymptom(std::shared_ptr<Symptom> newSymptom);
    void addSeverity(double newSeverity);
    double getSeverity();
    std::vector<std::shared_ptr<Symptom>> getSymptoms();
    unsigned int getSymptomsID();
    void evolve(std::vector<std::shared_ptr<Symptom>> allSymptoms);
    double getEvolveSymptomProbability(std::shared_ptr<Symptom> symptom);
};

class VirusManager {
  private:
    std::vector<std::shared_ptr<zpr::Symptom>> symptoms_;
    std::vector<double> mortality_;
    std::vector<double> contagiousness_;
    std::vector<std::shared_ptr<zpr::Symptom>> initialSymptoms_;

  public:
    VirusManager() : symptoms_(), mortality_(), contagiousness_() {}
    void addSymptom(std::shared_ptr<Symptom> newSymptom);
    void addMortality(double newMortality);
    void addContagiousness(double newContagiousness);
    std::vector<std::shared_ptr<Symptom>> getSymptoms();
    std::shared_ptr<Symptom> getSymptomByName(std::string name);
    std::vector<double> getMortality();
    std::vector<double> getContagiousness();
    double getMortalityByID(unsigned int id);
    double getContagiousnessByID(unsigned int id);
    void addInitialSymptom(std::shared_ptr<Symptom> symptom);
    std::shared_ptr<Symptom> getInitialSymptom();
};

}  // namespace zpr
#endif