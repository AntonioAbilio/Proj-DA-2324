#include "WaterManager.h"


/* Data Parsing Begin */

/**
 * @brief Calls the other data parsing functions
 */
void WaterManager::parseData() {
    std::ifstream in;

    // Open the file using the provided path.
    in.open("../inputFiles/MadeiraDataSet/Reservoirs_Madeira.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Reservoirs.csv.\n";
        return;
    }
    processReservoirs(in);
    in.close();

    in.open("../inputFiles/MadeiraDataSet/Stations_Madeira.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Stations.csv.\n";
        return;
    }
    processPumps(in);
    in.close();

    in.open("../inputFiles/MadeiraDataSet/Cities_Madeira.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Cities.csv.\n";
        return;
    }
    processCities(in);
    in.close();

    // ToDo remove
    /*std::cout << "\nWaterReservoirMap\n";

    for (auto e : this->waterReservoirMap) std::cout << e.first << "\n";

    std::cout << "\nWaterPumpMap\n";

    for (auto e : this->waterPumpMap) std::cout << e.first << "\n";

    std::cout << "\nwaterCityMap\n";

    for (auto e : this->waterCityMap) std::cout << e.first << "\n";*/

    in.open("../inputFiles/MadeiraDataSet/Pipes_Madeira.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Pipes.csv.\n";
        return;
    }
    processPipes(in);
    in.close();

    // Test graph begin

    /*for (Vertex<WaterElement*>* vertex : this->waterNetwork.getVertexSet()){
        std::cout << " " << vertex->getInfo()->getCode() << "\n";
    }*/

}

/**
 * @brief Frees the data on program exit
 */
void WaterManager::dataGoBoom() {
    for (Vertex<WaterElement*>* waterVertex : waterNetwork.getVertexSet()){
        waterNetwork.removeVertex(waterVertex->getInfo());
    }
}

/**
 * @brief Data parsing for reservoirs.
 */
void WaterManager::processReservoirs(std::ifstream &in) {
    // ToDo remove
    //std::cout << "Now Reservoirs\n";

    std::string line;
    getline(in,line);
    while(getline(in, line)) {
        std::istringstream s(line);
        std::string reservoir, municipality, id, code, maximumDelivery;

        if (std::getline(s, reservoir, ',') && std::getline(s, municipality, ',') && std::getline(s, id, ',') && std::getline(s, code, ',') && std::getline(s, maximumDelivery, '\r')) {
            for (char& c : reservoir) c = (char) tolower(c);
            for (char& c : municipality) c = (char) tolower(c);
            for (char& c : id) c = (char) tolower(c);
            for (char& c : code) c = (char) tolower(c);
            for (char& c : maximumDelivery) c = (char) tolower(c);
        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        WR* waterReservoir = new WR(code, stoi(id), reservoir, municipality, std::stod(maximumDelivery));

        this->waterReservoirMap[code] = waterReservoir;

        if (!waterNetwork.addVertex(waterReservoir)){
            std::cout << "Error while adding vertex to graph.";
        }

        // ToDo remove
        //std::cout << reservoir << " " << municipality << " " << id << " " << code << " " << maximumDelivery << std::endl;
    }
}

/**
 * @brief Data parsing for pumps
 */
void WaterManager::processPumps(std::ifstream &in) {
    // ToDo remove
    //std::cout << "Now PumpingStations\n";

    std::string line;
    getline(in,line);
    while(getline(in, line)) {
        std::istringstream s(line);
        std::string id, code;
        if (std::getline(s, id, ',') && std::getline(s, code, '\r')) {
            for (char& c : id) c = (char) tolower(c);
            for (char& c : code) c = (char) tolower(c);

        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        PS* pumpingStation = new PS(code, std::stoi(id));

        this->waterPumpMap[code] = pumpingStation;

        if (!waterNetwork.addVertex(pumpingStation)){
            std::cout << "Error while adding vertex to graph.";
        }

        // ToDo remove
        //std::cout << id << " " << code << std::endl;
    }
}


/**
 * @brief Data parsing for cities / deliverySites
 */
void WaterManager::processCities(std::ifstream &in) {
    // ToDo remove
    //std::cout << "Now Cities\n";

    std::string line;
    getline(in,line);
    while(getline(in, line)) {
        std::istringstream s(line);
        std::string city, id, code, demand, population;
        if (std::getline(s, city, ',') && std::getline(s, id, ',') && std::getline(s, code, ',') && std::getline(s, demand, ',') && std::getline(s, population, '\r')) {
            for (char& c : city) c = (char) tolower(c);
            for (char& c : id) c = (char) tolower(c);
            for (char& c : code) c = (char) tolower(c);
            for (char& c : demand) c = (char) tolower(c);
            for (char& c : population) c = (char) tolower(c);

        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        std::string actualPopulation;
        for (char& c : population){
            if (isdigit(c)){
                actualPopulation += c;
            }
        }

        DS* deliverySite = new DS(code, stoi(id), city, std::stod(demand), std::stoi(actualPopulation));

        this->waterCityMap[code] = deliverySite;

        if (!waterNetwork.addVertex(deliverySite)){
            std::cout << "Error while adding vertex to graph.";
        }

        // ToDo remove
        //std::cout << city << " " << id << " " << code << " " << demand << " " << population << std::endl;
    }
}

/**
 * @brief Data parsing for pipes.
 */
void WaterManager::processPipes(std::ifstream &in) {
    // ToDo remove
    //std::cout << "Now Pipes\n";

    std::string line;
    getline(in,line);
    while(getline(in, line)) {
        std::istringstream s(line);
        std::string servicePointA, servicePointB, capacity, direction;
        if (std::getline(s, servicePointA, ',') && std::getline(s, servicePointB, ',') && std::getline(s, capacity, ',') && std::getline(s, direction, '\r')) {
            for (char& c : servicePointA) c = (char) tolower(c);
            for (char& c : servicePointB) c = (char) tolower(c);
            for (char& c : capacity) c = (char) tolower(c);
            for (char& c : direction) c = (char) tolower(c);

        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        std::string typeA;
        for (int i = 0; i < servicePointA.find_first_of('_'); i++){
            typeA += servicePointA[i];
        }

        std::string typeB;
        for (int i = 0; i < servicePointB.find_first_of('_'); i++){
            typeB += servicePointB[i];
        }

        WaterElement *waterElementA = nullptr;
        WaterElement* waterElementB = nullptr;

        if (typeA == "r"){
            waterElementA = this->waterReservoirMap.at(servicePointA);
        } else if (typeA == "ps"){
            waterElementA = this->waterPumpMap.at(servicePointA);
        } else if (typeA == "c"){
            waterElementA = this->waterCityMap.at(servicePointA);
        }

        if (typeB == "r"){
            waterElementB = this->waterReservoirMap.at(servicePointB);
        } else if (typeB == "ps"){
            waterElementB = this->waterPumpMap.at(servicePointB);
        } else if (typeB == "c"){
            waterElementB = this->waterCityMap.at(servicePointB);
        }

        if (waterElementA == nullptr || waterElementB == nullptr){
            std::cerr << "Problem with finding vertex in maps.\n";
            return;
        }

        if (!this->waterNetwork.addEdge(waterElementA, waterElementB, std::stod(capacity))){
            std::cerr << "Problem while adding an edge to the graph\n";
            return;
        }

        if (std::stoi(direction)){
            if (!this->waterNetwork.addEdge(waterElementB, waterElementA, std::stod(capacity))){
                std::cerr << "Problem while adding an edge to the graph\n";
                return;
            }
        }

        // ToDo remove
        //std::cout << "\nFound water element: " << waterElementA->getCode() << " and found water element B: " << waterElementB->getCode() << "\n";

        // ToDo remove
        //std::cout << "ServicePointA " << typeA << std::endl;

        // ToDo remove
        //std::cout << servicePointA << " " << servicePointB << " " << capacity << " " << direction << std::endl;
    }
}

/* Data Parsing End */

// T2.2
void WaterManager::listwaterNeeds(){
    // TODO
    std::cout << "AAA";

    // 1. run maxFlow algorithm
    // 2. Compare maxflow with demand (for each city)
}

