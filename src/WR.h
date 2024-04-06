#ifndef PROJ_DA_1_WR_H
#define PROJ_DA_1_WR_H

#include "WaterElement.h"

class WR : public WaterElement{

private:
    std::string reservoir;
    std::string municipality;
    int maxDelivery;

public:
    WR(const std::string &code, int id, const std::string &reservoir, const std::string &municipality, double maxDelivery);
    const std::string &getReservoir() const;
    void setReservoir(const std::string &reservoir);
    const std::string &getMunicipality() const;
    void setMunicipality(const std::string &municipality);
    double getMaxDelivery() const;
    void setMaxDelivery(double maxDelivery);
};


#endif //PROJ_DA_1_WR_H
