#include "DS.h"

using namespace std;

DS::DS(const string &code, int id, const string &city, double demand, int population, double currentFlow)
        : WaterElement(code, id) {
    this->city = city;
    this->demand = demand;
    this->population = population;
    this->currentFlow = currentFlow;
}

const string &DS::getCity() const {
    return city;
}

void DS::setCity(const string &city) {
    DS::city = city;
}

double DS::getCurrentFlow() const {
    return currentFlow;
}

void DS::setCurrentFlow(double flow) {
    DS::currentFlow = flow;
}

double DS::getDemand() const {
    return demand;
}

void DS::setDemand(double demand) {
    DS::demand = demand;
}

int DS::getPopulation() const {
    return population;
}

void DS::setPopulation(int population) {
    DS::population = population;
}
