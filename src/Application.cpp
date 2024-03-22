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
            std::cout << "\nPlease specify the city's code:";
            std::getline(std::cin >> std::ws, cityCode); // Use std::ws to consume whitespaces
            std::cout << "\n";

            // Show maximum flow to specified city.
            waterManager.maximumFlowSpecificCities(cityCode);

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
    clearScreen();

    //Code here

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


    std::cout << "Water reservoir to be removed\n";
    // TODO: Different option for choosing water reservoir (not just using code)

    clearScreen();
    std::string wr_code;
    std::cout << "Please input water reservoir code: ";
    std::cin >> wr_code;

    clearScreen();
    waterManager.listCitiesAffectedByReservoirRemoval(wr_code);

    showGoBackMenu(4, "List cities affected by reservoir removal."); // At the end make a call to goBackMenu()
}

// T3.2
void Application::listCitiesAffectedByMaintenance(){
    clearScreen();

    //Code here

    showGoBackMenu(5, "List cities affected by maintenance."); // At the end make a call to goBackMenu()
}

// T3.3
void Application::listCitiesAffectedByPipeRupture(){
    clearScreen();

    //Code here

    showGoBackMenu(6, "List cities affected by pipe rupture."); // At the end make a call to goBackMenu()
}


