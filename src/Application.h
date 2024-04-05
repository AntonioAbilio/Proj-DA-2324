#ifndef PROJ_DA_1_APLICATIONMANAGER_H
#define PROJ_DA_1_APLICATIONMANAGER_H

#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "WaterManager.h"

/**@brief Class that manages the menu.*/
class Application{
public:
    Application(std::string env){
        waterManager.parseData();
        this->env = env;
    }

    void run(int processedKey);
    void clearScreen();
    static void delay(long sleepTime);
    static int processKey(const std::string& option);
    void outputToFile(std::string header, std::string text);

private:

    WaterManager waterManager;

    // WaterManager options
    std::string showMainMenu();

    void showGoBackMenu(int option, const std::string& functionName);

    void maxWater(); // T2.1

    void listwaterNeeds(); // T2.2

    void balaceNetworkLoad(); // T2.3

    void listCitiesAffectedByReservoirRemoval(); // T3.1

    void listCitiesAffectedByMaintenance(); // T3.2

    void listCitiesAffectedByPipeRupture(); // T3.3

    // Attributes
    std::string env; // System environment variable
};

#endif //PROJ_DA_1_APLICATIONMANAGER_H
