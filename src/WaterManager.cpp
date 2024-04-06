#include "WaterManager.h"

/* Data Parsing Begin */

/**
 * @brief Calls the other data parsing functions
 */
void WaterManager::parseData(bool useMadeira) {
    std::ifstream in;

    // Open the file using the provided path.

    if (useMadeira){
        in.open("../inputFiles/MadeiraDataSet/Reservoirs.csv");
    } else {
        in.open("../inputFiles/LargeDataSet/Reservoirs.csv");
    }
    if (!in.is_open()) {
        std::cout << "Unable to open Reservoirs.csv.\n";
        return;
    }
    processReservoirs(in);
    in.close();


    if (useMadeira){
        in.open("../inputFiles/MadeiraDataSet/Stations.csv");
    } else {
        in.open("../inputFiles/LargeDataSet/Stations.csv");
    }
    if (!in.is_open()) {
        std::cout << "Unable to open Stations.csv.\n";
        return;
    }
    processPumps(in);
    in.close();

    if (useMadeira){
        in.open("../inputFiles/MadeiraDataSet/Cities.csv");
    } else {
        in.open("../inputFiles/LargeDataSet/Cities.csv");
    }
    if (!in.is_open()) {
        std::cout << "Unable to open Cities.csv.\n";
        return;
    }
    processCities(in);
    in.close();

    if (useMadeira){
        in.open("../inputFiles/MadeiraDataSet/Pipes.csv");
    } else {
        in.open("../inputFiles/LargeDataSet/Pipes.csv");
    }
    if (!in.is_open()) {
        std::cout << "Unable to open Pipes.csv.\n";
        return;
    }
    processPipes(in);
    in.close();
}

/**
 * @brief Frees the data on program exit
 */
void WaterManager::dataGoBoom() {
    for (Vertex<WaterElement *> *waterVertex: waterNetwork.getVertexSet()) {
        waterNetwork.removeVertex(waterVertex->getInfo());
    }
}

/**
 * @brief Data parsing for reservoirs.
 */
void WaterManager::processReservoirs(std::ifstream &in) {
    std::string line;
    getline(in, line);
    while (getline(in, line)) {
        std::istringstream s(line);
        std::string reservoir, municipality, id, code, maximumDelivery;

        if (std::getline(s, reservoir, ',') && std::getline(s, municipality, ',') && std::getline(s, id, ',') &&
            std::getline(s, code, ',') && std::getline(s, maximumDelivery, '\r')) {
            for (char &c: reservoir) c = (char) tolower(c);
            for (char &c: municipality) c = (char) tolower(c);
            for (char &c: id) c = (char) tolower(c);
            for (char &c: code) c = (char) tolower(c);
            for (char &c: maximumDelivery) c = (char) tolower(c);
        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        WR *waterReservoir = new WR(code, stoi(id), reservoir, municipality, std::stod(maximumDelivery));

        this->waterReservoirMap[code] = waterReservoir;

        if (!waterNetwork.addVertex(waterReservoir)) {
            std::cout << "Error while adding vertex to graph.";
        }
    }
}

/**
 * @brief Data parsing for pumps
 */
void WaterManager::processPumps(std::ifstream &in) {
    std::string line;
    getline(in, line);
    while (getline(in, line)) {
        std::istringstream s(line);
        std::string id, code;
        if (std::getline(s, id, ',') && std::getline(s, code, '\r')) {
            for (char &c: id) c = (char) tolower(c);
            for (char &c: code) c = (char) tolower(c);

        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        PS *pumpingStation = new PS(code, std::stoi(id));

        this->waterPumpMap[code] = pumpingStation;

        if (!waterNetwork.addVertex(pumpingStation)) {
            std::cout << "Error while adding vertex to graph.";
        }
    }
}


/**
 * @brief Data parsing for cities / deliverySites
 */
void WaterManager::processCities(std::ifstream &in) {
    std::string line;
    getline(in, line);
    while (getline(in, line)) {
        std::istringstream s(line);
        std::string city, id, code, demand, population;
        if (std::getline(s, city, ',') && std::getline(s, id, ',') && std::getline(s, code, ',') &&
            std::getline(s, demand, ',') && std::getline(s, population, '\r')) {
            for (char &c: city) c = (char) tolower(c);
            for (char &c: id) c = (char) tolower(c);
            for (char &c: code) c = (char) tolower(c);
            for (char &c: demand) c = (char) tolower(c);
            for (char &c: population) c = (char) tolower(c);

        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }

        std::string actualPopulation;
        for (char &c: population) {
            if (isdigit(c)) {
                actualPopulation += c;
            }
        }

        DS *deliverySite = new DS(code, stoi(id), city, std::stod(demand), std::stoi(actualPopulation), 0.0);

        this->waterCityMap[code] = deliverySite;

        if (!waterNetwork.addVertex(deliverySite)) {
            std::cout << "Error while adding vertex to graph.";
        }
    }
}

/**
 * @brief Data parsing for pipes.
 */
void WaterManager::processPipes(std::ifstream &in) {
    std::string line;
    getline(in, line);
    while (getline(in, line)) {
        std::istringstream s(line);
        std::string servicePointA, servicePointB, capacity, direction;
        if (std::getline(s, servicePointA, ',') && std::getline(s, servicePointB, ',') &&
            std::getline(s, capacity, ',') && std::getline(s, direction, '\r')) {
            for (char &c: servicePointA) c = (char) tolower(c);
            for (char &c: servicePointB) c = (char) tolower(c);
            for (char &c: capacity) c = (char) tolower(c);
            for (char &c: direction) c = (char) tolower(c);
        } else {
            std::cerr << "Invalid line in the CSV file" << line << std::endl;
        }
        std::string typeA;
        for (int i = 0; i < servicePointA.find_first_of('_'); i++) {
            typeA += servicePointA[i];
        }
        std::string typeB;
        for (int i = 0; i < servicePointB.find_first_of('_'); i++) {
            typeB += servicePointB[i];
        }

        WaterElement *waterElementA = nullptr;
        WaterElement *waterElementB = nullptr;

        if (typeA == "r") {
            waterElementA = this->waterReservoirMap.at(servicePointA);
        } else if (typeA == "ps") {
            waterElementA = this->waterPumpMap.at(servicePointA);
        } else if (typeA == "c") {
            waterElementA = this->waterCityMap.at(servicePointA);
        }

        if (typeB == "r") {
            waterElementB = this->waterReservoirMap.at(servicePointB);
        } else if (typeB == "ps") {
            waterElementB = this->waterPumpMap.at(servicePointB);
        } else if (typeB == "c") {
            waterElementB = this->waterCityMap.at(servicePointB);
        }

        if (waterElementA == nullptr || waterElementB == nullptr) {
            std::cerr << "Problem with finding vertex in maps.\n";
            return;
        }

        if (!std::stoi(direction)) {
            if (!this->waterNetwork.addBidirectionalEdge(waterElementA, waterElementB, std::stod(capacity))) {
                std::cerr << "Problem while adding an edge to the graph\n";
                return;
            }
        } else {
            if (!this->waterNetwork.addEdge(waterElementA, waterElementB, std::stod(capacity))) {
                std::cerr << "Problem while adding an edge to the graph\n";
                return;
            } else pipesSize++;
        }
    }
}

/* Data Parsing End */


double getTotVertex(Vertex<WaterElement *> *adj, DS *deliverySite) {
    double tot = 0.0;
    for (Edge<WaterElement *> *edge: adj->getIncoming()) {
        tot += edge->getFlow();
    }
    return tot;
}

/**
 * @brief Helper function. Checks if a given delivery site already has the needed demand.
 * @details Its time complexity in the worst case is O(E) where E is the amount of edges that are
 * @details incoming and outgoing of a given delivery Site.
 *
 * @param adj - This is the vertex that contains the delivery Site.
 * @param deliverySite - This is the delivery site.
 *
 * @return If the demand has been fulfilled then it returns true, false otherwise.
 **/
bool demandFulfilled(Vertex<WaterElement *> *adj, DS *deliverySite) {
    return getTotVertex(adj, deliverySite) >= deliverySite->getDemand();
}

/**
 * @brief Helper function. Checks if there exists an augmenting path.
 * @details This function does a Breadth First Search (BFS) and tries to construct a path from source node to a target node.
 * @details The path in question can either be a normal path or it can be a residual path.
 * @details The time complexity for this function is O(V + V + E). This is because first we
 * @details iterate over every vertex and set visited to false. After this we perform a BFS
 * @details which iterates over the graph's vertexes (V) and for each one, over it's outgoing edges (E).
 *
 * @param source - This is the starting node.
 * @param target - This is the ending node.
 *
 * @return If it manages to find a path then it return true otherwise it returns false.
 **/
bool WaterManager::existsAugmentingPath(WaterElement *&source, WaterElement *&target) {
    Vertex<WaterElement *> *sourceVertex = waterNetwork.findVertex(source);
    Vertex<WaterElement *> *targetVertex = waterNetwork.findVertex(target);

    for (auto &vertex: waterNetwork.getVertexSet()) {
        vertex->setVisited(false);
    }

    sourceVertex->setVisited(true);
    std::queue<Vertex<WaterElement *> *> q;
    q.push(sourceVertex);

    while (!q.empty() && !targetVertex->isVisited()) {
        auto v = q.front();
        q.pop();

        if (v->isProcessing()) continue;

        // Process outgoing edges
        for (auto &e: v->getAdj()) {
            Vertex<WaterElement *> *adj = e->getDest();
            if (!adj->isVisited() && (e->getWeight() - e->getFlow() > 0)) {
                auto *isDS = dynamic_cast<DS *>(adj->getInfo());

                if (isDS) {
                    if (demandFulfilled(adj, isDS)) {
                        continue;
                    }
                }

                adj->setVisited(true);
                adj->setPath(e);
                q.push(adj);
            }
        }

        // Process incoming edges
        for (auto &e: v->getIncoming()) {
            Vertex<WaterElement *> *adj = e->getOrig();

            // Residual Network Edge.
            if (!adj->isVisited() && e->getFlow() > 0) {
                adj->setVisited(true);
                adj->setPath(e);
                q.push(adj);
            }
        }
    }

    return targetVertex->isVisited();
}

/**
 * @brief Helper function.
 * @details Beginning from the target vertex we are going to follow the previously saved path
 * @details to determine what is the minimum residual capacity of the path.
 * @details The time complexity for this function is O(V). This is because in the worst case we
 * @details need to go trough V vertexes (from the target to the source node).
 *
 * @param source - This is the ending node.
 * @param target - This is the starting node.
 *
 *
 * @return Minimum residual capacity of the path that goes from target to source.
 **/
double WaterManager::findMinResidualAlongPath(WaterElement *&source, WaterElement *&target) {
    Vertex<WaterElement *> *sourceVertex = waterNetwork.findVertex(source);

    // Note -> currentVertex starts at the target node and follows the path until it reaches the source node.
    Vertex<WaterElement *> *currentVertex = waterNetwork.findVertex(target);

    double minFlow = INFINITY;

    auto *isDS2 = dynamic_cast<DS *>(sourceVertex->getInfo());
    if (isDS2) {
        minFlow = std::min(minFlow, (isDS2->getDemand() - getTotVertex(sourceVertex, isDS2)));
    }

    while (currentVertex != sourceVertex) {
        auto *isDS = dynamic_cast<DS *>(currentVertex->getInfo());
        if (isDS) {
            minFlow = std::min(minFlow, (isDS->getDemand() - getTotVertex(currentVertex, isDS)));
        }

        auto e = currentVertex->getPath();
        if (e->getDest() == currentVertex) {
            minFlow = std::min(minFlow, e->getWeight() - e->getFlow());
            currentVertex = e->getOrig();
        } else {
            minFlow = std::min(minFlow, e->getFlow());
            currentVertex = e->getDest();
        }

    }
    return minFlow;
}

/**
 * @brief Helper function.
 * @details Beginning from the target vertex we are going to follow the previously saved path
 * @details and add (in case the path belongs to the normal graph) or subtract
 * @details (in case we have found a path in the residual graph) the minimum residual capacity.
 * @details The time complexity for this function is O(V). This is because in the worst case we
 * @details need to go trough V vertexes (from the target to the source node).
 *
 * @param source - This is the ending node.
 * @param target - This is the starting node.
 * @param minFlow - Minimum residual capacity.
 **/
void WaterManager::augmentFlowAlongPath(WaterElement *&source, WaterElement *&target, double minFlow) {
    Vertex<WaterElement *> *sourceVertex = waterNetwork.findVertex(source);
    Vertex<WaterElement *> *targetVertex = waterNetwork.findVertex(target);

    auto &currentVertex = targetVertex;

    while (currentVertex != sourceVertex) {
        auto e = currentVertex->getPath();
        double flow = e->getFlow();
        auto *isDS = dynamic_cast<DS *>(currentVertex->getInfo());
        if (e->getDest() == currentVertex) {
            e->setFlow(flow + minFlow);

            if (isDS) {
                isDS->setCurrentFlow(getTotVertex(currentVertex, isDS));
            }

            currentVertex = e->getOrig();
        } else {
            e->setFlow(flow - minFlow);

            if (isDS) {
                isDS->setCurrentFlow(getTotVertex(currentVertex, isDS));
            }

            currentVertex = e->getDest();
        }
    }
}


/**
 * @brief Maximum flow across all cities.
 * @details This functions uses a slightly modified Edmonds-Karp algorithm to
 * find the maximum amount of flow that can reach all cities. The modifications
 * are the addition of a super source and a super sink and the use of a variable
 * named currentFlow that helps us check if a given city already has it's demand fulfilled.
 * @details The time complexity for this functions is O(X + Y + Z) = O(V + VE + VE^2) = O(VE^2) where
 * X is the time complexity related to the addition of the superWaterReservoir (super source) and the
 * superDeliverySite (super sink) (X = V), Y is the time complexity of the for loops that set the flow
 * of edges and the currentFlow of vertexes to zero (Y = VE) and finally Z is the time complexity
 * for the Edmonds-Karp Algorithm for Max Flow (Z = VE^2).
 **/
std::string WaterManager::maximumFlowAllCities() {
    std::ostringstream oss;

    // Add superSource and superTarget
    WaterElement *superWaterReservoir = new WR("superWR", -1, "superWR", "none", 0);
    WaterElement *superDeliverySite = new DS("superDS", -1, "superDS", 0, 0, 0);
    waterNetwork.addVertex(superWaterReservoir);
    waterNetwork.addVertex(superDeliverySite);

    // Calculate the max delivery for the superWaterReservoir.
    for (const auto &waterReservoir: waterReservoirMap) {
        if (this->waterNetwork.findVertex(waterReservoir.second)->isProcessing())
            continue;
        ((WR *) superWaterReservoir)->setMaxDelivery(
                ((WR *) superWaterReservoir)->getMaxDelivery() + waterReservoir.second->getMaxDelivery());
        waterNetwork.addEdge(superWaterReservoir, waterReservoir.second, waterReservoir.second->getMaxDelivery());
    }

    // Calculate the demand for the superDeliverySite.
    for (const auto &city: waterCityMap) {
        ((DS *) superDeliverySite)->setDemand(((DS *) superDeliverySite)->getDemand() + city.second->getDemand());
        waterNetwork.addEdge(city.second, superDeliverySite, city.second->getDemand());
    }

    // Set flow to 0.
    for (Vertex<WaterElement *> *&vertex: waterNetwork.getVertexSet()) {
        auto *isDS = dynamic_cast<DS *>(vertex->getInfo());

        if (isDS) isDS->setCurrentFlow(0.0);

        if (!vertex->isProcessing()) vertex->setProcesssing(false);

        for (Edge<WaterElement *> *&edge: vertex->getAdj()) edge->setFlow(0);

    }

    while (existsAugmentingPath(superWaterReservoir, superDeliverySite)) {
        double minFlow = findMinResidualAlongPath(superWaterReservoir, superDeliverySite);
        augmentFlowAlongPath(superWaterReservoir, superDeliverySite, minFlow);
    }

    double tot = 0.0;
    for (const auto &city: waterCityMap) {
        tot += city.second->getCurrentFlow();
        oss << "The city " << city.second->getCity() << " has a maximum flow of " << city.second->getCurrentFlow()
            << " cubic meters per second.\n";
    }
    oss << "The total of flow is " << tot << ".\n";

    for (Vertex<WaterElement *> *vertex: waterNetwork.getVertexSet()) {
        for (Edge<WaterElement *> *edge: vertex->getIncoming()) {
            if (edge->getDest()->getInfo()->getCode() == "superDS") {
                waterNetwork.removeEdge(edge->getOrig()->getInfo(), edge->getDest()->getInfo());
            }
        }
    }

    waterNetwork.removeVertex(superWaterReservoir);
    waterNetwork.removeVertex(superDeliverySite);
    return oss.str();
}

/**
 * @brief Maximum flow to a specific city
 * @details This function depends on the function above [maximumFlowAllCities()]
 * @details The time complexity for this functions is is O(n + Y) = O(V + VE + VE^2) = O(VE^2) where
 * n is the time complexity for the worst case of finding the city and Y is the time
 * complexity of the maximumFlowAllCities() function.
 **/
std::string WaterManager::maximumFlowSpecificCities(std::string idCode) {
    std::ostringstream oss;
    size_t pos;

    for (auto &c: idCode) {
        c = (char) tolower(c);
    }

    std::string cityCode = cityCodePrefix;
    if ((pos = idCode.find('_')) != std::string::npos) {

        if (!std::regex_match(idCode.substr(0, pos + 1), std::regex(cityCodePrefix, std::regex::icase))) {
            oss << "\nNo pumping station with id/code " << idCode << " was found.\n";
            return oss.str();
        }

        for (size_t i = pos + 1; i < idCode.size(); i++) {
            cityCode += idCode[i];
        }
    } else {
        cityCode += idCode;
    }

    if (!std::regex_match(cityCode, std::regex(cityCodeRegex))) {
        bool validCity = false;

        for (auto possibleCity: waterCityMap) {
            if (possibleCity.second->getCity() == idCode) {
                validCity = true;
                cityCode = possibleCity.first;
            }
        }

        if (!validCity) {
            oss << "\nNo cities with id/code/name " << idCode << " was found.\n";
            return oss.str();
        }
    }

    auto iter = waterCityMap.find(cityCode);

    if (iter == waterCityMap.end()) {
        oss << "\nNo cities with id/code/name " << idCode << " was found.\n";
        return oss.str();
    }

    std::string cityToFind = iter->second->getCity();
    std::string res = maximumFlowAllCities(); // Get the results from the Max Flow Algorithm

    size_t begin = res.find(cityToFind);
    size_t end = res.find('\n', begin);

    oss << "The city " << res.substr(begin, end - begin) << "\n";
    return oss.str();
}

/**
 * @brief Helper function
 * @details This function takes two string and checks the differences between them.
 * The complexity for string::find is unspecified but according to c++ reference it's O(n).
 * Total complexity for function is in the worst case O(n^2).
 * @param before The old string.
 * @param after The new string.
 *
 **/
std::string checkDifferences(const std::string &before, const std::string &after) {
    std::ostringstream oss;

    std::istringstream bef_SS(before);
    std::istringstream aft_SS(after);

    std::string lineBefore;
    std::string lineAfter;
    while (getline(bef_SS, lineBefore) && getline(aft_SS, lineAfter)) {

        if (std::regex_match(lineBefore, std::regex("The total of flow is [0-9]+."))) continue;

        if (lineBefore != lineAfter) {

            // Find the name of the city.
            size_t beginDSName = 9;
            size_t endDSName = lineBefore.find("has");
            std::string deliverySiteName = lineBefore.substr(beginDSName, endDSName - beginDSName);

            // Find where the flow number begins.
            size_t begin_flow = endDSName + 22;

            // Find where the flow number ends [before changes].
            size_t before_end_flow = lineBefore.find("cubic");
            std::string DS_Flow_Before = lineBefore.substr(begin_flow, before_end_flow - begin_flow);

            // Find where the flow number ends [after changes].
            size_t after_end_flow = lineAfter.find("cubic");
            std::string DS_Flow_After = lineAfter.substr(begin_flow, after_end_flow - begin_flow);

            oss << "\nThe city " << deliverySiteName << "had a flow of " << DS_Flow_Before
                << "cubic meters per second" << " but now has a flow of " << DS_Flow_After
                << "cubic meters per second.\n";
        }
    }

    if (oss.str().empty()) {
        oss << "The water supply network has not been affected. Every city still has the same amount of flow.\n";
    }

    return oss.str();
}

/**
 * @brief This function aids in checking which cities are affected if a given pump is taken offline.
 * @details It first executes the algorithm to find the maximum flow to all cities,
 * "deactivates" the specified pump if it is found, executes the max flow algorithm again and
 * it compares the results.
 * @details The Time Complexity for this function is (VE^2) which is also the time complexity of the max flow
 * algorithm.
 **/
std::string WaterManager::citiesAffectedByMaintenance_SpecificPump(std::string idCode) {
    std::ostringstream oss;
    size_t pos;

    std::string pumpCode = pumpingStationCodePrefix;
    if ((pos = idCode.find('_')) != std::string::npos) {

        if (!std::regex_match(idCode.substr(0, pos + 1), std::regex(pumpingStationCodePrefix, std::regex::icase))) {
            oss << "\nNo pumping station with id/code " << idCode << " was found.\n";
            return oss.str();
        }

        for (size_t i = pos + 1; i < idCode.size(); i++) {
            pumpCode += idCode[i];
        }

    } else {
        pumpCode += idCode;
    }

    if (!std::regex_match(pumpCode, std::regex(pumpingStationCodeRegex))) {
        oss << "\nNo pumping station with id/code " << idCode << " was found.\n";
        return oss.str();
    }

    auto iter = waterPumpMap.find(pumpCode);

    if (iter == waterPumpMap.end()) {
        oss << "\nNo pumping station with id/code " << idCode << " was found.\n";
        return oss.str();
    }

    std::vector<Edge<WaterElement *>> outgoingEdges;
    std::vector<Edge<WaterElement *>> incomingEdges;

    std::string flowBeforeRemoval = maximumFlowAllCities();

    Vertex<WaterElement *> *PSVertex = waterNetwork.findVertex(iter->second);

    PSVertex->setProcesssing(true); // Disable the vertex.

    std::string flowAfterRemoval = maximumFlowAllCities();

    oss << checkDifferences(flowBeforeRemoval, flowAfterRemoval);

    PSVertex->setProcesssing(false); // Disable the vertex.

    return oss.str();
}


/**
 * @brief This function aids in checking which cities are affected if each one of the pumps is taken offline.
 * @details This function uses the previous function citiesAffectedByMaintenance_SpecificPump().
 * @details The Time Complexity for this function is O(XY) = O(VE^2) where X is
 * in the worst case the number of total amount of pumps that are in the graph
 * and Y is the time complexity for the previously used function, O(VE^2).
 **/
std::string WaterManager::citiesAffectedByMaintenance_AllPumps() {
    std::ostringstream oss;

    for (const auto &pumps: this->waterPumpMap) {

        oss << "Removing pumping station with code " << pumps.first << ". Result:\n";
        oss << citiesAffectedByMaintenance_SpecificPump(pumps.first);
        oss << "\n\n";
    }

    return oss.str();
}

// T2.2
/**
 * @brief Lists the cities that are in need of water, by checking the actual flow delivered to them
 * @details Time Complexity: O(V * E^2), because of Edmonds-Karp Algorithm
 */
void WaterManager::listWaterNeeds() {
    maximumFlowAllCities();  // Run Edmonds-Karp algorithm

    for (auto ds: waterCityMap) { // Only check flow for delivery sites
        int receivedFlow = 0;
        WaterElement *ds_to_we = ds.second;
        Vertex<WaterElement *> *v = waterNetwork.findVertex(ds_to_we);
        if (v == nullptr) {
            std::cout << "An error has occurred...\n";
            return;
        }
        for (auto e: v->getIncoming()) receivedFlow += e->getFlow();
        int demand = ds.second->getDemand();
        if (receivedFlow < demand) {
            std::cout << ds.second->getCity() << " (" << ds.first << ") needs more water:"
                      << "\n- Demand: " << demand
                      << "\n- Actual flow: " << receivedFlow
                      << "\n- Deficit: " << demand - receivedFlow << "\n\n";
        }
    }
}

// T3.1
/**
 * @brief Lists the cities that are in need of water, by checking the actual flow delivered to them
 * @details This function runs the Edmond's Karp algorithm to determine the maximum flow before removing
 * a water reservoir and after this if the given water reservoir exists it disables it and re-runs the Edmonds-Karp
 * Algorithm to check the flow after the removal.
 * @details Time Complexity: O(V * E^2), because of Edmonds-Karp Algorithm
 */
void WaterManager::listCitiesAffectedByReservoirRemoval(std::string idCode) {
    std::ostringstream oss;
    size_t pos;

    std::string wrCode = reservoirCodePrefix;
    if ((pos = idCode.find('_')) != std::string::npos) {

        if (!std::regex_match(idCode.substr(0, pos + 1), std::regex(reservoirCodePrefix, std::regex::icase))) {
            oss << "\nNo pumping station with id/code " << idCode << " was found.\n";
            return;
        }

        for (size_t i = pos + 1; i < idCode.size(); i++) {
            wrCode += idCode[i];
        }
    } else {
        wrCode += idCode;
    }

    if (!std::regex_match(wrCode, std::regex(reservoirCodeRegex))) {
        oss << "\nNo water reservoir with id/code " << idCode << " was found.\n";
        return;
    }

    // Find the water reservoir
    WR *WRToRemove;
    try {
        WRToRemove = waterReservoirMap.at(wrCode);
    } catch (std::out_of_range &ofr) {
        std::cout << "\nNo water reservoir with id/code " << wrCode << " was found.\n";
        return;
    }

    Vertex<WaterElement *> *waterReservoir = waterNetwork.findVertex(WRToRemove);
    if (waterReservoir == nullptr) {
        std::cout << "Vertex " << WRToRemove->getCode() << " not found\n";
        return;
    }

    maximumFlowAllCities();  // Get flow before removal
    std::unordered_map<std::string, double> citiesOldFlow;
    for (const auto &ds: waterCityMap) {
        citiesOldFlow.insert(std::make_pair(ds.first, ds.second->getCurrentFlow()));
    }

    if (waterReservoir->isProcessing()) {
        std::cout << "\nThe water reservoir with code/id " << wrCode << " has already been disabled.\n";
        return;
    }

    // Instead of removing the water reservoir let's set isProcessing to True.
    waterReservoir->setProcesssing(true);
    this->disabledWaterReservoirs.push_back(waterReservoir);

    maximumFlowAllCities();  // Get flow after removal

    for (const auto &ds: waterCityMap) { // Only check flow for delivery sites
        int receivedFlow = 0;
        WaterElement *ds_to_we = ds.second;
        Vertex<WaterElement *> *v = waterNetwork.findVertex(ds_to_we);
        if (v == nullptr) {
            std::cout << "An error has occurred...\n";
            return;
        }
        for (auto e: v->getIncoming()) receivedFlow += e->getFlow();
        int demand = ds.second->getDemand();
        if (receivedFlow < demand) {
            std::cout << ds.second->getCity() << " (" << ds.first << ") needs more water:"
                      << "\n- Old flow: " << citiesOldFlow[ds.first]
                      << "\n- New flow: " << receivedFlow << "\n\n";
        }
    }
}

/**
 * @brief "Reenable" all reservoirs
 * @details Time Complexity: O(V), where V are all the disabled water reservoirs
 */
void WaterManager::resetWaterReservoirs() {
    for (Vertex<WaterElement *> *&currentlyDisabledWR: this->disabledWaterReservoirs) {
        currentlyDisabledWR->setProcesssing(false);
    }
    this->disabledWaterReservoirs.clear();
}

std::vector<Vertex<WaterElement *> *> WaterManager::getDisabledWaterReservoirs() {
    return disabledWaterReservoirs;
}

/**
 * @brief Helper function to calculate maximum flow.
 * @details This function calls the maximumFlowAllCities function (Edmond's Karp) and calculates
 * the updated maximum flow of each city, storing it into a map of delivery sites as keys and the
 * respective flow value.
 * @details Time Complexity: O(VE²), where V is the number of water elements in the system and E
 * is the number of pipes.
 *
 * @return A map containing the delivery sites as keys and the respective updated maximum flow
 * values.
 **/
std::map<DS *, double> WaterManager::auxMaxFlow() {

    if (maximumFlowAllCities().empty()) std::cout << "ERROR! maxFlow failed.";

    std::map<DS *, double> cities;

    for (const auto &city: waterCityMap) {
        auto cityVertex = waterNetwork.findVertex(city.second);
        auto *isDS = dynamic_cast<DS *>(cityVertex->getInfo());

        if (isDS) {
            cities.insert({isDS, isDS->getCurrentFlow()});
        }

    }

    return cities;
}

/**
 * @brief Determine the cities affected by pipe ruptures based on a given city code.
 * @details This function examines each pipe connected to the specified city and calculates
 * the maximum flow after simulating the rupture of each pipe. It records the affected cities
 * and the deficit in water supply caused by each pipe rupture.
 * @details Time Complexity: O(VE³), where V is the number of vertices (water elements) and E is the
 * number of edges (pipes) in the network. This is because for each pipe examined, an Edmond's
 * Karp maximum flow algorithm is performed, and in the worst case, a city is connected to all
 * the pipes of the system.
 *
 * @param cityCode The city code of the city to be analyzed.
 * @return A map containing information about the affected cities and the deficit in water supply
 * caused by each pipe rupture. The keys are strings representing the pipes, and the values are
 * vectors of pairs, where each pair contains the affected city code and the deficit in water supply.
 **/
std::map<std::string, std::vector<std::pair<std::string, double>>> WaterManager::CitiesAffectedByPipeRupture(std::string &idCode) {
    size_t pos;
    std::map<std::string, std::vector<std::pair<std::string, double>>> result;

    for (auto &c: idCode) {
        c = (char) tolower(c);
    }

    std::string cityCode = cityCodePrefix;
    if ((pos = idCode.find('_')) != std::string::npos) {

        if (!std::regex_match(idCode.substr(0, pos + 1), std::regex(cityCodePrefix, std::regex::icase))) {
            std::cout << "\nNo pumping station with id/code " << idCode << " was found.\n";
            return result;
        }

        for (size_t i = pos + 1; i < idCode.size(); i++) {
            cityCode += idCode[i];
        }
    } else {
        cityCode += idCode;
    }

    if (!std::regex_match(cityCode, std::regex(cityCodeRegex))) {
        bool validCity = false;

        for (auto possibleCity: waterCityMap) {
            if (possibleCity.second->getCity() == idCode) {
                validCity = true;
                cityCode = possibleCity.first;
            }
        }

        if (!validCity) {
            std::cout << "Error: City not found!\n";
            return result;
        }
    }

    // Find the target city vertex
    Vertex<WaterElement *> *targetCityVertex = waterNetwork.findVertex(waterCityMap.at(cityCode));

    // If the target city vertex is not found, return an empty result
    if (targetCityVertex == nullptr) {
        std::cout << "Error: City not found!\n";
        return result;
    }

    // Calculate original flow for all cities.
    std::map<DS *, double> originalFlows = auxMaxFlow();

    for (Vertex<WaterElement *> *we: waterNetwork.getVertexSet()) {
        // Loop through all the edges connected to the target city vertex
        for (Edge<WaterElement *> *pipe: we->getAdj()) {
            // Simulate pipe rupture by setting its flow capacity to 0
            auto destination = pipe->getDest()->getInfo();
            auto origin = pipe->getOrig()->getInfo();
            double weight = pipe->getWeight();
            bool bidirectional = false;
            waterNetwork.removeEdge(pipe->getOrig()->getInfo(), pipe->getDest()->getInfo());
            if (waterNetwork.removeEdge(destination, origin)) bidirectional = true;
            // Calculate the maximum flow after the pipe rupture
            std::map<DS *, double> maxFlows = auxMaxFlow();

            // Check if the desired water supply cannot be met for any city
            for (const auto &city: maxFlows) {
                try {
                    if ((city.second < city.first->getDemand()) && (originalFlows.at(city.first) > city.second) &&
                        (targetCityVertex->getInfo()->getCode() == city.first->getCode())) {
                        // Record the affected city and the deficit in water supply
                        std::string affectedCity = city.first->getCity(); // Remove the prefix 'c_'
                        double deficit = city.first->getDemand() - city.second;
                        std::ostringstream oss;

                        oss << " from service point " << origin->getCode() << " to service point "
                            << destination->getCode();
                        result[oss.str()].emplace_back(affectedCity, deficit);
                    }
                } catch (std::out_of_range){
                    std::cout << "city not found!" << std::endl;
                }
            }

            // Restore the original flow capacity of the pipe
            waterNetwork.addEdge(origin, destination, weight);
            if (bidirectional) waterNetwork.addEdge(destination, origin, weight);
        }
    }
    return result;
}

/**
 * @brief Determine the cities affected by pipe ruptures.
 * @details This function examines each pipe in the water network and calculates the maximum flow
 * after simulating the rupture of each pipe. It records the affected cities and the deficit in
 * water supply caused by each pipe rupture.
 * @details Time Complexity: O(VE³), where V is the number of vertices (water elements) and E is the
 * number of edges (pipes) in the network. This is because for each pipe examined, an Edmond's
 * Karp maximum flow algorithm is performed.
 *
 * @return A map containing information about the affected cities and the deficit in water supply
 * caused by each pipe rupture. The keys are strings representing the pipes (from origin to
 * destination), and the values are vectors of pairs, where each pair contains the affected city
 * and the deficit in water supply.
 **/
std::map<std::string, std::vector<std::pair<std::string, double>>> WaterManager::CitiesAffectedByPipeRupture() {
    std::map<std::string, std::vector<std::pair<std::string, double>>> result;

    // Calculate original flows for all cities
    std::map<DS *, double> originalFlows = auxMaxFlow();

    for (Vertex<WaterElement *> *we: waterNetwork.getVertexSet()) {
        // Loop through all the edges connected to the target city vertex
        for (Edge<WaterElement *> *pipe: we->getAdj()) {
            // Simulate pipe rupture by setting its flow capacity to 0
            auto destination = pipe->getDest()->getInfo();
            auto origin = pipe->getOrig()->getInfo();
            double weight = pipe->getWeight();
            bool bidirectional = false;
            waterNetwork.removeEdge(pipe->getOrig()->getInfo(), pipe->getDest()->getInfo());
            if (waterNetwork.removeEdge(destination, origin)) bidirectional = true;
            // Calculate the maximum flow after the pipe rupture
            std::map<DS *, double> maxFlows = auxMaxFlow();

            // Check if the desired water supply cannot be met for any city
            for (const auto &city: maxFlows) {
                try {
                    if ((city.second < city.first->getDemand()) && (originalFlows.at(city.first) > city.second)) {
                        // Record the affected city and the deficit in water supply
                        std::string affectedCity = city.first->getCity(); // Remove the prefix 'c_'
                        double deficit = city.first->getDemand() - city.second;
                        std::ostringstream oss;

                        oss << " from service point " << origin->getCode() << " to service point "
                            << destination->getCode();
                        result[oss.str()].emplace_back(affectedCity, deficit);
                    }
                } catch (std::out_of_range){
                    std::cout << "city not found!" << std::endl;
                }
            }

            // Restore the original flow capacity of the pipe
            waterNetwork.addEdge(origin, destination, weight);
            if (bidirectional) waterNetwork.addEdge(destination, origin, weight);
        }
    }
    return result;
}

/**
 * @brief Saves the pipes in the water network.
 * @details This function iterates over all edges in the water network and saves them into a vector of edges.
 * @details Each edge consists of a source vertex, a destination vertex, and the weight representing the capacity of the pipe.
 * @details Has time complexity of O(E), since it visits every edge at least once.
 * @return A vector containing all the pipes in the water network.
 */
std::vector<Edge<WaterElement *>> WaterManager::savePipes() {
    std::vector<Edge<WaterElement *>> res;
    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            Edge<WaterElement *> e = {edge->getOrig(), edge->getDest(), edge->getWeight()};
            res.push_back(e);
        }
    }
    return res;
}

/**
 * @brief Restores the pipes in the water network to their original state.
 * @details This function removes all edges in the water network and then adds the edges from the provided vector.
 * @details The provided vector contains the original state of the pipes, including source vertices, destination vertices, and weights.
 * @details Has time complexity of O(E), since it visits every edge at least once.
 * @param original A vector containing the original state of the pipes.
 */
void WaterManager::restorePipes(std::vector<Edge<WaterElement *>> original) {
    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            waterNetwork.removeEdge(edge->getOrig()->getInfo(), edge->getDest()->getInfo());
        }
    }

    for (auto edge: original) {
        waterNetwork.addEdge(edge.getOrig()->getInfo(), edge.getDest()->getInfo(), edge.getWeight());
    }
}

/**
 * @brief Calculates the average capacity of all pipes in the water network.
 * @details This function iterates over all edges in the water network and computes the sum of their weights, representing their capacity.
 * @details It then divides the sum of capacities by the total number of pipes to obtain the average capacity.
 * @details The time complexity of this function is O(V + E), where V is the number of vertices and E is the number of edges in the water network.
 * @return The average capacity of all pipes in the water network.
 */
double WaterManager::avgCapacity() {
    double sumCapacity = 0.0;
    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            sumCapacity += edge->getWeight();
        }
    }
    return sumCapacity / static_cast<double>(pipesSize);
}

/**
 * @brief Calculates the average difference between the capacity and flow of each pipe in the water network.
 * @details This function iterates over all edges in the water network and computes the absolute difference between their capacity and flow.
 * @details Additionally, it updates the maximum difference found during the iteration.
 * @details The time complexity of this function is O(V + E), where V is the number of vertices and E is the number of edges in the water network.
 *
 * @param maxDifference Reference to a variable that will store the maximum difference found during the iteration.
 * @return The average difference between the capacity and flow of each pipe.
 */
double WaterManager::avgDifference(double &maxDifference) {
    double sumDifference = 0.0;
    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            double difference = abs(edge->getWeight() - edge->getFlow());
            maxDifference = std::max(maxDifference, difference);
            sumDifference += difference;
        }
    }
    return sumDifference / static_cast<double>(pipesSize);
}

/**
 * @brief Calculates the variance of the differences between the capacity and flow of each pipe in the water network.
 * @details This function computes the average difference first using the avgDifference() function.
 * @details The time complexity of this function is O(V + E), where V is the number of vertices and E is the number of edges in the water network.
 *
 * @return The variance of the differences between the capacity and flow of each pipe.
 */
double WaterManager::variance() {
    double maxDifference = 0.0;
    double avg = avgDifference(maxDifference);
    double sumVariance = 0.0;
    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            double difference = abs(edge->getWeight() - edge->getFlow());
            sumVariance += std::pow((difference - avg), 2);
        }
    }
    return std::sqrt(sumVariance / static_cast<double>(pipesSize) + 1);
}

struct CompareDifference {
    bool operator()(double a, double b) const {
        return a > b; // Decreasing Order
    }
};

/**
 * @brief Performs a balancing algorithm using sorting distribution technique.
 * @details This method balances the flow across the network by redistributing flow between pipes based on sorting distribution technique.
 * @details It saves the original pipe configuration, computes initial metrics such as max difference, average difference
 * @details Computes the difference between capacity and flow for each pipe and stores this value into a multimap of descending order.
 * @details It iterates through the map (size/2 times), sending the overflow difference of the highest to the lowest. Using a sequential and a reverse iterator.
 * @details After the redistribution, it computes final metrics and displays the changes in max difference, average difference, and variance.
 * @details Additionally, it displays the changes in maximum flow for each city before and after the balancing process.
 * @details It has time complexity equal to the maximumFlowAllCities function, since it uses it twice.
 * @details (
 * @details     Time complexity is O(X + Y + Z).
 *
 * @details     X - Time complexity related to the addition of the superWaterReservoir and the superDeliverySite. X = V
 *
 * @details     Y - The for loops that set the flow of edges and the currentFlow of vertexes to zero. Y = VE
 *
 * @details     Z - Edmonds-Karp Algorithm for Max Flow. Z = VE^2
 * @details )
 * @details The method then restores the original pipe configuration.
 */
void WaterManager::balancingAlgorithmSortingDistribution() {
    auto originalEdges = savePipes();
    std::map<DS *, double> initialFlows = auxMaxFlow();
    double initialMaxDifference = 0.0;
    double initialAvgDifference = avgDifference(initialMaxDifference);
    double initialVariance = variance();

    std::multimap<double, Edge<WaterElement *> *, CompareDifference> differenceMap;

    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            double difference = abs(edge->getWeight() - edge->getFlow());
            differenceMap.insert({difference, edge});

        }
    }
    int count = 0;
    auto itl = differenceMap.begin();
    itl++;
    auto itr = differenceMap.rbegin();
    while (count < differenceMap.size() / 2 && itl != differenceMap.end() && itr != differenceMap.rend()) {
        double redistributed = itl->first - itr->first;
        itl->second->setWeight(itl->second->getWeight() - redistributed);
        itr->second->setWeight(itr->second->getWeight() + redistributed);
        ++count;
        ++itr;
        ++itl;
    }
    std::map<DS *, double> finalFlows = auxMaxFlow();
    double finalMaxDifference = 0.0;
    double finalAvgDifference = avgDifference(finalMaxDifference);
    double finalVariance = variance();
    std::cout << std::setw(20) << "Initial Max Diff" << std::setw(20) << "Final Max Diff" << std::setw(20)
              << "Initial Avg Diff"
              << std::setw(20) << "Final Avg Diff" << std::setw(20) << "Initial Variance" << std::setw(20)
              << "Final Variance"
              << std::setw(20) << initialMaxDifference << std::setw(20) << finalMaxDifference << std::setw(20)
              << initialAvgDifference
              << std::setw(20) << finalAvgDifference << std::setw(20) << initialVariance << std::setw(20)
              << finalVariance << std::endl;

    // Output table header
    std::cout << std::setw(20) << "City" << std::setw(35) << "Max Flow (Before)" << std::setw(25) << "Max Flow (After)"
              << std::endl;
    for (auto city: waterCityMap) {
        // Output city data
        try {
            std::cout << std::setw(20) << city.second->getCity() << std::setw(20) << initialFlows.at(city.second)
                      << " m^3/s"
                      << std::setw(20) << finalFlows.at(city.second) << " m^3/s"
                      << std::endl;
        } catch (std::out_of_range){
            std::cout << "city not found!" << std::endl;
        }
    }

    restorePipes(originalEdges);
}

/**
 * @brief Performs a balancing algorithm using neighbor distribution technique.
 * @details This method balances the flow across the network by redistributing flow between neighboring pipes based on neighbor distribution technique.
 * @details Access the neighbouring edges of a vertex and checks if the difference between capacity and flow is lower than its own, if it is, saves the neighbor edge and computes a redistribution function in order to balance.
 *
 * @details It saves the original pipe configuration, computes initial metrics such as max difference, average difference, and variance, then iteratively redistributes flow between neighboring pipes.
 * @details After the redistribution, it computes final metrics and displays the changes in max difference, average difference, and variance.
 * @details Additionally, it displays the changes in maximum flow for each city before and after the balancing process.
 * @details The method then restores the original pipe configuration.
 * @details It has time complexity equal to the maximumFlowAllCities function, since it uses it twice.
 * @details (
 * @details     Time complexity is O(X + Y + Z).
 *
 * @details     X - Time complexity related to the addition of the superWaterReservoir and the superDeliverySite. X = V
 *
 * @details     Y - The for loops that set the flow of edges and the currentFlow of vertexes to zero. Y = VE
 *
 * @details     Z - Edmonds-Karp Algorithm for Max Flow. Z = VE^2
 * @details )
 */
void WaterManager::balancingAlgorithmNeighborDistribution() {
    auto originalEdges = savePipes();
    std::map<DS *, double> initialFlows = auxMaxFlow();
    double initialMaxDifference = 0.0;
    double initialAvgDifference = avgDifference(initialMaxDifference);
    double initialVariance = variance();

    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            double difference = abs(edge->getWeight() - edge->getFlow());
            std::vector<Edge<WaterElement *> *> neighboringPipes;

            for (auto adj: edge->getDest()->getAdj()) {
                if (abs(adj->getWeight() - adj->getFlow()) < difference) {
                    neighboringPipes.push_back(adj);
                }
            }

            for (auto neighbor: neighboringPipes) {
                double thisDifference = abs(neighbor->getWeight() - neighbor->getFlow());
                difference = abs(edge->getWeight() - edge->getFlow());
                if (thisDifference < difference) {
                    double redistributed = difference - thisDifference;
                    edge->setWeight(edge->getWeight() - redistributed);
                    neighbor->setWeight(neighbor->getWeight() + redistributed);


                }
            }
        }
    }
    std::map<DS *, double> finalFlows = auxMaxFlow();
    double finalMaxDifference = 0.0;
    double finalAvgDifference = avgDifference(finalMaxDifference);
    double finalVariance = variance();
    std::cout << std::setw(20) << "Initial Max Diff" << std::setw(20) << "Final Max Diff" << std::setw(20)
              << "Initial Avg Diff"
              << std::setw(20) << "Final Avg Diff" << std::setw(20) << "Initial Variance" << std::setw(20)
              << "Final Variance"
              << std::setw(20) << initialMaxDifference << std::setw(20) << finalMaxDifference << std::setw(20)
              << initialAvgDifference
              << std::setw(20) << finalAvgDifference << std::setw(20) << initialVariance << std::setw(20)
              << finalVariance << std::endl;

    // Output table header
    std::cout << std::setw(20) << "City" << std::setw(35) << "Max Flow (Before)" << std::setw(25) << "Max Flow (After)"
              << std::endl;
    for (const auto &city: waterCityMap) {
        // Output city data
        try {
        std::cout << std::setw(20) << city.second->getCity() << std::setw(20) << initialFlows.at(city.second) << " m^3/s"
                  << std::setw(20) << finalFlows.at(city.second) << " m^3/s"
                  << std::endl;
    } catch (std::out_of_range){
            std::cout << "city not found!" << std::endl;
    }
    }
    restorePipes(originalEdges);
}

/**
 * @brief Performs a balancing algorithm using average distribution technique.
 * @details This method balances the flow across the network by setting the weight of each pipe to the average capacity of all pipes.
 * @details It saves the original pipe configuration, computes initial metrics such as max difference, average difference, and variance, then sets the weight of each pipe to the average capacity.
 * @details After setting the weights, it computes final metrics and displays the changes in max difference, average difference, and variance.
 * @details Additionally, it displays the changes in maximum flow for each city before and after the balancing process.
 * @details The method then restores the original pipe configuration.
 * @details It has time complexity equal to the maximumFlowAllCities function, since it uses it twice.
 * @details (
 * @details     Time complexity is O(X + Y + Z).
 *
 * @details     X - Time complexity related to the addition of the superWaterReservoir and the superDeliverySite. X = V
 *
 * @details     Y - The for loops that set the flow of edges and the currentFlow of vertexes to zero. Y = VE
 *
 * @details     Z - Edmonds-Karp Algorithm for Max Flow. Z = VE^2
 * @details )
 */
void WaterManager::balancingAlgorithmAverageDistribution() {
    auto originalEdges = savePipes();
    std::map<DS *, double> initialFlows = auxMaxFlow();
    double initialMaxDifference = 0.0;
    double initialAvgDifference = avgDifference(initialMaxDifference);
    double initialVariance = variance();
    double avgCapacity_ = avgCapacity();

    for (auto we: waterNetwork.getVertexSet()) {
        for (auto edge: we->getAdj()) {
            edge->setWeight(avgCapacity_);
        }
    }
    std::map<DS *, double> finalFlows = auxMaxFlow();
    double finalMaxDifference = 0.0;
    double finalAvgDifference = avgDifference(finalMaxDifference);
    double finalVariance = variance();
    std::cout << std::setw(20) << "Initial Max Diff" << std::setw(20) << "Final Max Diff" << std::setw(20)
              << "Initial Avg Diff"
              << std::setw(20) << "Final Avg Diff" << std::setw(20) << "Initial Variance" << std::setw(20)
              << "Final Variance"
              << std::setw(20) << initialMaxDifference << std::setw(20) << finalMaxDifference << std::setw(20)
              << initialAvgDifference
              << std::setw(20) << finalAvgDifference << std::setw(20) << initialVariance << std::setw(20)
              << finalVariance << std::endl;

    // Output table header
    std::cout << std::setw(20) << "City" << std::setw(35) << "Max Flow (Before)" << std::setw(25) << "Max Flow (After)"
              << std::endl;
    for (const auto &city: waterCityMap) {
        // Output city data
        try {
            std::cout << std::setw(20) << city.second->getCity() << std::setw(20) << initialFlows.at(city.second)
                      << " m^3/s"
                      << std::setw(20) << finalFlows.at(city.second) << " m^3/s"
                      << std::endl;
        } catch (std::out_of_range) {
            std::cout << "city not found!" << std::endl;
        }
    }
    restorePipes(originalEdges);
}
