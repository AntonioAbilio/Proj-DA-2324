#include "Application.h"

void Application::run(int processedKey) {
    L1:
    clearScreen();
    while (processedKey == -1) {
        clearScreen();
        processedKey = processKey(showMainMenu());
    }

    switch (processedKey) {
        case 1:
            maxWater(); // T2.1
            std::cout << 1;
            break;
        case 2:
            listWaterNeeds(); // T2.2
            std::cout << 2;
            break;
        case 3:
            balaceNetworkLoad(); // T2.3
            std::cout << 3;
            break;
        case 4:
            listCitiesAffectedByReservoirRemoval(); // T3.1
            break;
        case 5:
            listCitiesAffectedByMaintenance(); // T3.2
            std::cout << 5;
            break;
        case 6:
            listCitiesAffectedByPipeRupture(); // T3.3
            std::cout << 4;
            break;
        case 7:
            waterManager.dataGoBoom();
            std::cout << "Thank you very much and Bye-Bye.\n";
            break;
        default:
            goto L1;
    }
}

/**
 * @brief clears (or simulates clearing) the screen, depending on the OS used
 */
void Application::clearScreen() {
    if (env == "win")
        L1:
        std::cout << "\n\n\n\n\n\n\n\n\n"
                     "\n\n\n\n\n\n\n\n\n"
                     "\n\n\n\n\n\n\n\n\n"
                     "\n\n\n\n\n\n\n\n\n";
    else if (env == "unix")
        if (system("clear") == -1) goto L1;
}

/**
 * @brief Small delay for design purposes
 */
void Application::delay(long sleepMS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
}


/** @brief Handles the exception thrown by std::stoi. */
int Application::processKey(const std::string &option) {
    try {
        int intOPT = std::stoi(option);
        if (intOPT <= 0 || option.size() > 1 || intOPT > 7) throw std::invalid_argument("NegativeNumber");
        return intOPT;
    } catch (std::invalid_argument &argument) {
        std::cout << "\n* Error while parsing option, please input a valid numeric option. *\n";
        delay(2000);
        return -1;
    }
}

/**
 * @brief Shows the main menu
 */
std::string Application::showMainMenu() {
    std::string option;
    std::cout << "Select an operation you would like to do:\n\n"
              << "1 - Maximum amount of water that can reach each or a specific city.\n"
              << "2 - Show water needs.\n"
              << "3 - Balance network load.\n"
              << "4 - List cities affected by reservoir removal.\n"
              << "5 - List cities affected by pump maintenance.\n"
              << "6 - List cities affected by pipe rupture.\n"
              << "7 - Exit.\n";

    std::cout << "\nInput: ";
    std::cin >> option;
    std::cout << "\n";
    return option;
}

void Application::showGoBackMenu(int functionNumber, const std::string &functionName) {
    L1:
    std::cout << "\nPress enter to continue";
    std::string a;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, a);
    clearScreen();
    std::cout << "What would you like to do next:\n"
              << "1 - Return to main menu.\n"
              << "2 - (again) " << functionName << "\n";

    std::string opt;
    std::cout << "\nInput: ";
    std::cin >> opt;
    std::cout << "\n";

    int processedKey = processKey(opt);

    switch (processedKey) {
        case -1:
            goto L1;
        case 1:
            throw std::invalid_argument("-1");
        case 2:
            throw std::invalid_argument(std::to_string(functionNumber));
        default:
            std::cout << "\n* Error while parsing option, please input a valid numeric option. *\n";
            goto L1;
    }
}

/**
 * @brief Helper function to output text to file.
 * @details This function adds a header composed of the current time plus a specified
 * header and the actual text that is due to be written.
 * **/
void Application::outputToFile(std::string header, std::string text) {
    std::ofstream out("../outputFiles/functions.txt", std::ios::app);

    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the current time to a struct tm
    std::tm *localTime = std::localtime(&currentTime);

    out << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << " -> "
        << header << "\n\n" << text << "\n\n";

    out.close();
}

// T2.1
void Application::maxWater() {
    restartMaxWater:
    clearScreen();
    std::string targetOpt;
    std::cout << "What would you like to do?\n\n"
              << "1 - See maximum amount of water that can reach all cities.\n"
              << "2 - See maximum amount of water that can reach a specific city.\n"
              << "\nInput: ";
    std::cin >> targetOpt;

    clearScreen();

    std::string cityCode;
    std::string res;
    switch (processKey(targetOpt)) {
        case 1:
            // Show all maximum flow to all cities.
            res = waterManager.maximumFlowAllCities();
            std::cout << res;
            outputToFile("Exercise T2.1 - Max Flow All Cities", res);
            break;
        case 2:
            std::cout << "Please specify the city's code or id or city name\n"
                      << "id example -> [Input: 7]\n"
                      << "code example -> [Input: C_7]\n"
                      << "name example -> [Input: câmara de lobos]\n"
                      << "\nInput: ";

            std::getline(std::cin >> std::ws, cityCode);
            std::cout << "\n";
            clearScreen();

            // Show maximum flow to specified city.
            res = waterManager.maximumFlowSpecificCities(cityCode);
            std::cout << res;
            outputToFile("Exercise T2.1 Max Flow to city " + cityCode, res);
            break;
        default:
            goto restartMaxWater;
    }

    showGoBackMenu(1, "Maximum amount of water that can reach each or a specific city.");
}

// T2.2
void Application::listWaterNeeds() {
    clearScreen();
    waterManager.listWaterNeeds();
    showGoBackMenu(2, "Show water needs.");
}

// T2.3
void Application::balaceNetworkLoad() {
    restartBalanceNetworkLoad:
    clearScreen();

    std::string targetOpt;
    std::cout << "What would you like to do?\n\n"
              << "1 - Balancing algorithm based of redistribution by sorting of differences of pipe capacity and pipe flow.\n"
              << "2 - Balancing algorithm based of redistribution of differences of pipe capacity and pipe flow between neighboring pipes.\n"
              << "3 - Balancing algorithm based of all pipes having the average difference of the initial water network.\n"
              << "\nInput: ";
    std::cin >> targetOpt;

    clearScreen();
    switch (processKey(targetOpt)) {
        case 1:
            waterManager.balancingAlgorithmSortingDistribution();
            break;
        case 2:
            waterManager.balancingAlgorithmNeighborDistribution();
            break;
        case 3:
            waterManager.balancingAlgorithmAverageDistribution();
            break;
        default:
            goto restartBalanceNetworkLoad;
    }
    showGoBackMenu(3, "Balance network load.");
}

// T3.1
void Application::listCitiesAffectedByReservoirRemoval() {
    clearScreen();
    std::cout << "Please select which water reservoir you would like to disable?\n"
              << "id example -> [Input: 1]\n"
              << "code example -> [Input: R_1]\n"
              << "\nInput: ";
    std::string idCode;
    std::cin >> idCode;

    std::string opt = "y";

    clearScreen();

    do {
        waterManager.listCitiesAffectedByReservoirRemoval(idCode);
        std::cout << "\nWould you like to disable another water reservoir? (y/N)\n"
                  << "\nInput: ";
        std::cin >> opt;
        clearScreen();
        if (std::regex_match(opt, std::regex("n", std::regex_constants::icase))) break;
        if (!waterManager.getDisabledWaterReservoirs().empty())
            std::cout << "\nThe current disabled water reservoirs are:\n";
        for (Vertex<WaterElement *> *vWE: waterManager.getDisabledWaterReservoirs()) {
            std::cout << vWE->getInfo()->getId() << std::endl;
        }
        std::cout << "\nPlease select which water reservoir you would like to disable?\n"
                  << "id example -> [Input: 1]\n"
                  << "code example -> [Input: R_1]\n"
                  << "\nInput: ";
        std::cin >> idCode;
        std::cout << "\n";
    } while (true);

    this->waterManager.resetWaterReservoirs();

    showGoBackMenu(4, "List cities affected by reservoir removal.");
}

// T3.2
void Application::listCitiesAffectedByMaintenance() {
    restartLCAFBM:
    clearScreen();
    std::string targetOpt;
    std::cout << "What would you like to do?\n\n"
              << "1 - Remove a specific pumping station and check the affected delivery sites.\n"
              << "2 - Check for each removed pumping station the affected delivery sites.\n"
              << "\nInput: ";
    std::cin >> targetOpt;

    clearScreen();

    std::string idCode;
    switch (processKey(targetOpt)) {
        case 1:
            std::cout << "Please provide the id/code for the pumping station that is to be removed.\n"
                      << "id example -> [Input: 1]\n"
                      << "code example -> [Input: PS_1]\n"
                      << "\nInput: ";
            std::cin >> idCode;
            clearScreen();

            std::cout << waterManager.citiesAffectedByMaintenance_SpecificPump(idCode);
            break;
        case 2:
            std::cout << waterManager.citiesAffectedByMaintenance_AllPumps();
            break;
        default:
            goto restartLCAFBM;
    }
    showGoBackMenu(5, "List cities affected by pump maintenance.");
}

// T3.3
void Application::listCitiesAffectedByPipeRupture() {
    restartPipelineFailure:
    clearScreen();

    std::string targetOpt;
    std::cout << "What would you like to do?\n\n"
              << "1 - See cities affected by a pipe belonging to a specific city.\n"
              << "2 - See global results for pipeline failures.\n"
              << "\nInput: ";
    std::cin >> targetOpt;

    clearScreen();
    std::string idCode;
    std::map<std::string, std::vector<std::pair<std::string, double>>> result;
    switch (processKey(targetOpt)) {
        case 1:
            std::cout << "Please provide the id/code for the city.\n"
                      << "id example -> [Input: 1]\n"
                      << "code example -> [Input: C_1]\n"
                      << "name example -> [Input: câmara de lobos]\n"
                      << "\nInput: ";

            std::getline(std::cin >> std::ws, idCode);
            std::cout << "\n";
            result = waterManager.CitiesAffectedByPipeRupture(idCode);
            break;
        case 2:
            result = waterManager.CitiesAffectedByPipeRupture();
            break;
        default:
            goto restartPipelineFailure;
    }


    if (result.empty()) {
        std::cout << "No cities affected by pipe rupture found for the specified city code.\n";
    } else {
        // Iterate over the result map
        for (auto &it: result) {
            std::cout
                    << "-----------------------------------------------------------------------------------------------------------\n";
            std::cout << "The removal of pipe" << it.first << " would affect the following cities:\n";

            // Iterate over the affected cities for the current pipe
            int count = 0;
            for (const auto &city: it.second) {
                ++count;
                std::cout << "  " << count << ". City: " << city.first << ", Decrease in Demand: " << city.second
                          << " m^3/sec\n";
            }

        }
    }
    std::cout
            << "-----------------------------------------------------------------------------------------------------------\n";


    showGoBackMenu(6, "List cities affected by pipe rupture.");
}


