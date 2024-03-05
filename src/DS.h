#ifndef PROJ_DA_1_DS_H
#define PROJ_DA_1_DS_H

#include "WaterElement.h"
#include <string>

class DS : public WaterElement{

private:
    std::string city;
    double demand;
    int population;

public:
    DS(const std::string &code, int id, const std::string &city, double demand, int population);

    const std::string &getCity() const;

    void setCity(const std::string &city);

    double getDemand() const;

    void setDemand(double demand);

    int getPopulation() const;

    void setPopulation(int population);


};


#endif //PROJ_DA_1_DS_H
