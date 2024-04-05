#include "Application.h"

void Application::run(int processedKey) {
    L1:
    clearScreen();
    while (processedKey == -1){
        clearScreen();
        processedKey = processKey(showMainMenu());
    }

    switch (processedKey) {
        case 1:
            maxWater(); // T2.1
            std::cout << 1;
            break;
        case 2:
            listwaterNeeds(); // T2.2
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
            //dataGoBoom();
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
        if ( system("clear") == -1 ) goto L1;
}

/**
 * @brief Small delay for design purposes
 */
void Application::delay(long sleepMS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));
}


/** @brief Handles the exception thrown by std::stoi. */
int Application::processKey(const std::string& option) {
    try {
        int intOPT = std::stoi(option);
        if (intOPT <= 0 || option.size() > 1 || intOPT > 7) throw std::invalid_argument("NegativeNumber");
        return intOPT;
    } catch (std::invalid_argument& argument) {
        std::cout << "\n* Error while parsing option, please input a valid numeric option. *\n";
        delay(2000);
        return -1;
    }
}

/**
 * @brief Shows the main menu
 */
std::string Application::showMainMenu() {
    std::string opti;
    std::cout << "\nSelect an operation you would like to do:\n\n"
              << "1 - Maximum amount of water that can reach each or a specific city.\n"
              << "2 - Show water needs.\n"
              << "3 - Balance network load.\n"
              << "4 - List cities affected by reservoir removal.\n"
              << "5 - List cities affected by pipe maintenance.\n"
              << "6 - List cities affected by pipe rupture.\n"
              << "7 - Exit.\n";

    std::cout << "Input: ";
    std::cin >> opti;
    std::cout << "\n";
    return opti;
}

void Application::showGoBackMenu(int functionNumber, const std::string& functionName) {
    L1:
    std::cout << "\nPress enter to continue";
    std::string a;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, a);
    clearScreen();
    std::cout << "\n\nWhat would you like to do next:\n"
              << "1 - Return to main menu.\n"
              << "2 - (again) " << functionName << "\n";

    std::string opt;
    std::cout << "Input: ";
    std::cin >> opt;
    std::cout << "\n";

    int processedKey = processKey(opt);

    switch (processedKey) {
        case -1:
            goto L1;
        case 1:
            // This needs to return to main otherwise when the "new" run method called here returned the program would
            // still keep on executing...
            // This way we are replacing the variable in main and still calling the function run in main
            // the no-no function -> run(-1)
            throw std::invalid_argument("-1");
        case 2:
            // the no-no function -> run(functionNumber)
            throw std::invalid_argument(std::to_string(functionNumber));
        default:
            std::cout << "\n* Error while parsing option, please input a valid numeric option. *\n";
            goto L1;
    }
}

/* Some things to keep in mind.
 *
 * If your menu has sub-menus / sub-options they should appear like the main menu
 * You can use a simple cout and cin.
 *
 * Like this:
 * std::cout << "\nHow would you like to search for the target Airport ?\n"
 *             << "1 - Using it's code.\n"
 *             << "2 - Using it's name.\n"
 *             << "3 - Using the cities name (all airports considered).\n"
 *             << "4 - Using coordinates.\n"
 *             << "5 - Go back to main menu\n"
 *             << "Input: ";
 * std::cin >> targetOpt;
 *
 * Then you can make a call to processKey() like:
 *
 * processKey(targetOpt) and it will be converted to an int which can be used in:
 *
 *  a switch case... (use goto in default)
 *
 *  while (true) loop
 *
 * */

// T2.1
void Application::maxWater(){
restartMaxWater:
    clearScreen();

    std::string targetOpt;
    std::cout << "\nWhat would you like to do?\n"
              << "1 - See maximum amount of water that can reach all cities.\n"
              << "2 - See maximum amount of water that can reach a specific city.\n"
              << "Input: ";
    std::cin >> targetOpt;

    clearScreen();

    std::string cityCode;
    switch (processKey(targetOpt)) {
        case 1:
            // Show all maximum flow to all cities.
            std::cout << waterManager.maximumFlowAllCities();
            break;
        case 2:
            std::cout << "\nPlease specify the city's code or id or city name\n"
                      << "id example -> [Input: 1]\n"
                      << "code example -> [Input: PS_1]\n"
                      << "name example -> [Input: câmara de lobos]\n"
                      << "Input: ";



            std::getline(std::cin >> std::ws, cityCode); // Use std::ws to consume whitespaces
            std::cout << "\n";

            // Show maximum flow to specified city.
            std::cout << waterManager.maximumFlowSpecificCities(cityCode);

            break;
        default:
            goto restartMaxWater;
    }

    showGoBackMenu(1,"Maximum amount of water that can reach each or a specific city."); // At the end make a call to goBackMenu()
}

// T2.2
void Application::listwaterNeeds(){
    clearScreen();

    waterManager.listwaterNeeds();
    //Code here

    showGoBackMenu(2, "Show water needs."); // At the end make a call to goBackMenu()
}

// T2.3
void Application::balaceNetworkLoad(){
    restartbalanceNetworkLoad:
    clearScreen();


    std::string targetOpt;
    std::cout << "\nWhat would you like to do?\n"
              << "1 - Balancing algorithm based of redistribution by sorting of differences of pipe capacity and pipe flow.\n\n"
              << "2 - Balancing algorithm based of redistribution of differences of pipe capacity and pipe flow between neighboring pipes.\n"
              << "3 - Balancing algorithm based of all pipes having the average difference of the initial water network.\n\n"
              << "Input: ";
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
            goto restartbalanceNetworkLoad;
    }

    showGoBackMenu(3, "Balance network load."); // At the end make a call to goBackMenu()
}

// T3.1
void Application::listCitiesAffectedByReservoirRemoval(){
    clearScreen();

    /* You might be concerned about the network's resiliency. In this context, you should
    evaluate what happens in terms of the delivery capacity of the network if one specific water reservoir
    is out of commission. How are the various delivery sites affected? Your system must allow it to take as
    input the water reservoir that will be out of service and determine which cities are affected by having
    their water supply not being met. Your interface should thus allow the user to selectively remove one
    reservoir and list the affected cities whose water supply does not meet its demand. Could you think
    about an algorithm that only sometimes needs to run the entire Max-Flow algorithm from scratch to
    evaluate the impact of removing all the reservoirs, one at a time? Discuss this in your presentation.*/

    //1. User input: Water reservoir that will be out of service


    std::cout << "Which water reservoir would you like to remove?\n";
    // TODO: Different option for choosing water reservoir (not just using code)
    std::string wr_code;
    std::cout << "Input (code): ";
    std::cin >> wr_code;
    std::string remove_str;
    bool remove = false;
    clearScreen();
    std::cout << "Would you like to remove it permanently? (Y/n)\n";
    std::cin >> remove_str;
    if (remove_str == "Y" || remove_str == "y") remove = true;
    clearScreen();
    waterManager.listCitiesAffectedByReservoirRemoval(wr_code, remove);

    // TODO: Reflect changes in actual file?

    showGoBackMenu(4, "List cities affected by reservoir removal."); // At the end make a call to goBackMenu()
}

// T3.2
void Application::listCitiesAffectedByMaintenance(){
restartLCAFBM:
    clearScreen();

    std::string targetOpt;
    std::cout << "\nWhat would you like to do?\n"
              << "1 - Remove a specific pumping station and check the affected delivery sites.\n"
              << "2 - Check for each removed pumping station the affected delivery sites.\n"
              << "Input: ";
    std::cin >> targetOpt;

    clearScreen();

    std::string idCode;
    switch (processKey(targetOpt)) {
        case 1:

            std::cout << "\nPlease provide the id/code for the pumping station that is to be removed.\n"
                      << "id example -> [Input: 1]\n"
                      << "code example -> [Input: PS_1]\n"
                      << "Input: ";
            std::cin >> idCode;

            std::cout << waterManager.citiesAffectedByMaintenance_SpecificPipe(idCode);
            break;
        case 2:
            std::cout << waterManager.citiesAffectedByMaintenance_AllPipes();
            break;
        default:
            goto restartLCAFBM;
    }

    showGoBackMenu(5, "List cities affected by maintenance."); // At the end make a call to goBackMenu()
}

// T3.3
void Application::listCitiesAffectedByPipeRupture(){
    restartPipelineFailure:
    clearScreen();


    std::string targetOpt;
    std::cout << "\nWhat would you like to do?\n"
              << "1 - See cities affected by a pipe belonging to a specific city.\n"
              << "2 - See global results for pipeline failures.\n"
              << "Input: ";
    std::cin >> targetOpt;

    clearScreen();
    std::string cityCode;
    std::map<std::string, std::vector<std::pair<std::string, double>>> result;
    switch (processKey(targetOpt)) {
        case 1:

            std::cout << "\nPlease specify the city's code: ";
            std::getline(std::cin >> std::ws, cityCode); // Use std::ws to consume whitespaces
            std::cout << "\n";

            // Call the function to get cities affected by pipe rupture
            result = waterManager.CitiesAffectedByPipeRupture(cityCode);
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
        for (auto it = result.begin(); it != result.end(); ++it) {
            std::cout << "-----------------------------------------------------------------------------------------------------------\n";
            std::cout << "The removal of pipe " << it->first << " would affect the following cities:\n";

            // Iterate over the affected cities for the current pipe
            int count = 0;
            for (const auto& city : it->second) {
                ++count;
                std::cout << "  " << count << ". City: " << city.first << ", Decrease in Demand: " << city.second << " m^3/sec\n";
            }

        }
    }
    std::cout << "-----------------------------------------------------------------------------------------------------------\n";


    showGoBackMenu(6, "List cities affected by pipe rupture.");
}


