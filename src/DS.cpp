#include "DS.h"

using namespace std;
DS::DS(const string &code, int id, const string &city, double demand, double population) : WaterElement(code,
                                                                                                                  id){
    this->city = city;
    this->demand = demand;
    this->population = population;
}

const string &DS::getCity() const {
    return city;
}

void DS::setCity(const string &city) {
    DS::city = city;
}

double DS::getDemand() const {
    return demand;
}

void DS::setDemand(double demand) {
    DS::demand = demand;
}

double DS::getPopulation() const {
    return population;
}

void DS::setPopulation(double population) {
    DS::population = population;
}
