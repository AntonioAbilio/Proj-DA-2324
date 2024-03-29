#ifndef PROJ_DA_1_DS_H
#define PROJ_DA_1_DS_H

#include "WaterElement.h"
#include <string>

class DS : public WaterElement{

private:
    std::string city;
    double demand;
    double currentFlow;
    int population;

public:
    DS(const std::string &code, int id, const std::string &city, double demand, int population, double currentFlow);

    const std::string &getCity() const;

    void setCity(const std::string &city);

    double getCurrentFlow() const;

    void setCurrentFlow(double flow);

    double getDemand() const;

    void setDemand(double demand);

    int getPopulation() const;

    void setPopulation(int population);


};


#endif //PROJ_DA_1_DS_H
