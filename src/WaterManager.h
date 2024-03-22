#ifndef PROJ_DA_1_MENU_H
#define PROJ_DA_1_MENU_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <unordered_map>

#include "Graph.h"
#include "WaterElement.h"
#include "WR.h"
#include "PS.h"
#include "DS.h"

class WaterManager {
public:
    void parseData(); // Data Parsing.

    void dataGoBoom(); // Free allocated space.


    // Exercise T2.1
    bool existsAugmentingPath(WaterElement*& source, WaterElement*& target); //HelperFunction

    double findMinResidualAlongPath(WaterElement*& source, WaterElement*& target); // HelperFunction

    void augmentFlowAlongPath(WaterElement*& source, WaterElement*& target, double f); // HelperFunction

    std::string maximumFlowAllCities();
    std::string maximumFlowSpecificCities(std::string cityCode);

    std::map<std::string, std::vector<std::string>> CitiesAffectedByPipeRupture(std::string &city);

private:

    Graph<WaterElement*> waterNetwork;

    std::unordered_map<std::string, WR*> waterReservoirMap;

    std::unordered_map<std::string, PS*> waterPumpMap;

    std::unordered_map<std::string, DS*> waterCityMap;

    void processReservoirs(std::ifstream &in); // Parsing of Reservoirs.
    void processPumps(std::ifstream &in); // Parsing of Pumping Stations.
    void processCities(std::ifstream &in); // Parsing of Cities (Delivery Sites).
    void processPipes(std::ifstream &in); // Parsing of Pipes.


    std::map<DS * , double> auxMaxFlow();
};


#endif //PROJ_DA_1_MENU_H
