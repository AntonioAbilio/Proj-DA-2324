#include "WR.h"
using namespace std;

WR::WR(const string &code, int id, const string &reservoir, const string &municipality, double maxDelivery)
        : WaterElement(code, id) {
    this->reservoir = reservoir;
    this->municipality = municipality;
    this->maxDelivery = maxDelivery;
}

const string &WR::getReservoir() const {
    return reservoir;
}

void WR::setReservoir(const string &reservoir) {
    WR::reservoir = reservoir;
}

const string &WR::getMunicipality() const {
    return municipality;
}

void WR::setMunicipality(const string &municipality) {
    WR::municipality = municipality;
}

double WR::getMaxDelivery() const {
    return maxDelivery;
}

void WR::setMaxDelivery(double maxDelivery) {
    WR::maxDelivery = maxDelivery;
}
