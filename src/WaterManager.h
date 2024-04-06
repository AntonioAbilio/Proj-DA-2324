#ifndef PROJ_DA_1_MENU_H
#define PROJ_DA_1_MENU_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <unordered_map>
#include <map>
#include <cmath>
#include <iomanip>

#include "Graph.h"
#include "WaterElement.h"
#include "WR.h"
#include "PS.h"
#include "DS.h"

#define cityCodePrefix "c_"
#define reservoirCodePrefix "r_"
#define pumpingStationCodePrefix "ps_"

#define cityCodeRegex "c_[0-9]+"
#define reservoirCodeRegex "r_[0-9]+"
#define pumpingStationCodeRegex "ps_[0-9]+"

class WaterManager {
public:
    void parseData(); // Data Parsing.
    void dataGoBoom(); // Free allocated space.

    // Exercise T2.1
    bool existsAugmentingPath(WaterElement *&source, WaterElement *&target); //HelperFunction
    double findMinResidualAlongPath(WaterElement *&source, WaterElement *&target); // HelperFunction
    void augmentFlowAlongPath(WaterElement *&source, WaterElement *&target, double f); // HelperFunction
    std::string maximumFlowAllCities();

    std::string maximumFlowSpecificCities(std::string cityCode);

    // Exercise T2.2
    void listWaterNeeds();

    // Exercise T2.3
    double avgDifference(double &maxDifference);

    double avgCapacity();

    double variance();

    void restorePipes(std::vector<Edge<WaterElement *>> original);

    std::vector<Edge<WaterElement *>> savePipes();

    void balancingAlgorithmNeighborDistribution();

    void balancingAlgorithmSortingDistribution();

    void balancingAlgorithmAverageDistribution();

    // Exercise T3.1
    void listCitiesAffectedByReservoirRemoval(std::string wr_code);

    void resetWaterReservoirs();

    std::vector<Vertex<WaterElement *> *> getDisabledWaterReservoirs();

    // Exercise T3.2
    std::string citiesAffectedByMaintenance_SpecificPump(std::string idCode);

    std::string citiesAffectedByMaintenance_AllPumps();

    // Exercise T3.3
    std::map<std::string, std::vector<std::pair<std::string, double>>> CitiesAffectedByPipeRupture(std::string &idCode);

    std::map<std::string, std::vector<std::pair<std::string, double>>> CitiesAffectedByPipeRupture();

private:

    Graph<WaterElement *> waterNetwork; // Graph.
    std::unordered_map<std::string, WR *> waterReservoirMap; // Contains all water reservoirs.
    std::unordered_map<std::string, PS *> waterPumpMap; // Contains all pumping stations.
    std::unordered_map<std::string, DS *> waterCityMap; // Contains all delivery sites.

    void processReservoirs(std::ifstream &in); // Parsing of Reservoirs.
    void processPumps(std::ifstream &in); // Parsing of Pumping Stations.
    void processCities(std::ifstream &in); // Parsing of Cities (Delivery Sites).
    void processPipes(std::ifstream &in); // Parsing of Pipes.

    int pipesSize = 0;
    std::vector<Vertex<WaterElement *> *> disabledWaterReservoirs;

    std::map<DS *, double> auxMaxFlow();
};

#endif //PROJ_DA_1_MENU_H
