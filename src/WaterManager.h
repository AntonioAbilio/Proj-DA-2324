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

    void listwaterNeeds(); // T2.2

    // Exercise T2.1
    bool existsAugmentingPath(WaterElement*& source, WaterElement*& target); //HelperFunction
    double findMinResidualAlongPath(WaterElement*& source, WaterElement*& target); // HelperFunction
    void augmentFlowAlongPath(WaterElement*& source, WaterElement*& target, double f); // HelperFunction

    std::string maximumFlowAllCities();
    std::string maximumFlowSpecificCities(std::string cityCode);
    //3.3
    std::map<std::string, std::vector<std::pair<std::string, double>>> CitiesAffectedByPipeRupture(std::string &cityCode);
    std::map<std::string, std::vector<std::pair<std::string, double>>> CitiesAffectedByPipeRupture();
    //2.3
    double avgDifference(double &maxDifference);
    double avgCapacity();
    double variance();
    void restorePipes(std::vector<Edge<WaterElement*>> original);
    std::vector<Edge<WaterElement*>> savePipes();
    void balancingAlgorithmNeighborDistribution();
    void balancingAlgorithmSortingDistribution();
    void balancingAlgorithmAverageDistribution();

    // Exercise T3.1
    void removeWR(WR* wr, std::vector<Edge<WaterElement*>>* outgoing, std::vector<Edge<WaterElement*>>* incoming);
    void addWR(WR* wr, const std::vector<Edge<WaterElement*>>& outgoing, const std::vector<Edge<WaterElement*>>& incoming);
    void listCitiesAffectedByReservoirRemoval(std::string wr_code, bool remove);

    // Exercice T3.2
    void removePS(PS* ps, std::vector<Edge<WaterElement*>>* outgoing, std::vector<Edge<WaterElement*>>* incoming);
    void addPS(PS* ps, const std::vector<Edge<WaterElement*>>& outgoing, const std::vector<Edge<WaterElement*>>& incoming);
    std::string citiesAffectedByMaintenance_SpecificPipe(std::string idCode);
    std::string citiesAffectedByMaintenance_AllPipes();

    void dfsAffectedByRemoval(Vertex<WaterElement*>* v, std::vector<WaterElement*> &res);
    void updateFlow(Vertex<WaterElement*>* WR );

private:

    Graph<WaterElement*> waterNetwork;
    int pipesSize = 0;
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
