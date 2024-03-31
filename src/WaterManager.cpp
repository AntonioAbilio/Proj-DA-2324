#include <cmath>
#include "WaterManager.h"


/* Data Parsing Begin */

/**
 * @brief Calls the other data parsing functions
 */
void WaterManager::parseData() {
    std::ifstream in;

    // Open the file using the provided path.
    in.open("../inputFiles/LargeDataSet/Reservoir.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Reservoirs.csv.\n";
        return;
    }
    processReservoirs(in);
    in.close();

    in.open("../inputFiles/LargeDataSet/Stations.csv");
    if (!in.is_open()){
        std::cout << "Unable to open Stations.csv.\n";
        return;
    }
    processPumps(in);
    in.close();

    in.open("../inputFiles/LargeDataSet/Cities.csv");
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

    in.open("../inputFiles/LargeDataSet/Pipes.csv");
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

        DS* deliverySite = new DS(code, stoi(id), city, std::stod(demand), std::stoi(actualPopulation), 0.0);

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


double getTotVertex(Vertex<WaterElement*>* adj, DS* deliverySite){
    double tot = 0.0;
    for (Edge<WaterElement*>* edge : adj->getIncoming()){
        tot += edge->getFlow();
    }
    return tot;
}

/**
 * @brief Helper function. Checks if a given delivery site already has the needed demand.
 * @details It's time complexity in the worst case is O(E) where E is the amount of edges that are
 * @details incoming and outgoing of a given delivery Site.
 *
 * @param adj - This is the vertex that contains the delivery Site.
 * @param deliverySite - This is the delivery site.
 *
 * @return If the demand has been fulfilled then it returns true, false otherwise.
 **/
bool demandFulfilled(Vertex<WaterElement*>* adj, DS* deliverySite){
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
 *
 * @return If it manages to find a path then it return true otherwise it returns false.
 **/
bool WaterManager::existsAugmentingPath(WaterElement*& source, WaterElement*& target){
    Vertex<WaterElement*>* sourceVertex = waterNetwork.findVertex(source);
    Vertex<WaterElement*>* targetVertex = waterNetwork.findVertex(target);

    for(auto& vertex : waterNetwork.getVertexSet()){
        vertex->setVisited(false);
    }

    sourceVertex->setVisited(true);
    std::queue<Vertex<WaterElement*> *> q;
    q.push(sourceVertex);

    while(!q.empty() && !targetVertex->isVisited()){
        auto v = q.front();
        q.pop();

        // Process outgoing edges
        for(auto& e: v->getAdj()) {
            Vertex<WaterElement*>* adj = e->getDest();

            // Normal Path, the outgoing edge is not full.
            if (!adj->isVisited() && (e->getWeight() - e->getFlow() > 0)) {
                //std::cout << " new to queue " << adj->getInfo() << "\n";

                auto* isDS = dynamic_cast<DS*>(adj->getInfo());

                if (isDS){
                    if (demandFulfilled(adj, isDS)){
                        std::cout << "Found delivery site but already full " << isDS->getCity() << "\n";
                        continue;
                    }
                }

                adj->setVisited(true);
                adj->setPath(e);
                q.push(adj);
            }
        }

        // Process incoming edges
        for(auto& e: v->getIncoming()) {
            Vertex<WaterElement*>* adj = e->getOrig();

            // Residual Network Edge.
            if (!adj->isVisited() && e->getFlow() > 0) {
                //std::cout << " new to queue " << adj->getInfo() << "\n";
                adj->setVisited(true);
                adj->setPath(e);
                q.push(adj);
            }
        }
    }

    // Return true if a path to the target is found, false otherwise
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
double WaterManager::findMinResidualAlongPath(WaterElement*& source, WaterElement*& target){
    Vertex<WaterElement*>* sourceVertex = waterNetwork.findVertex(source);

    // Note -> currentVertex starts at the target node and follows the path until it reaches the source node.
    Vertex<WaterElement*>* currentVertex = waterNetwork.findVertex(target);

    double minFlow = INFINITY;

    auto* isDS2 = dynamic_cast<DS*>(sourceVertex->getInfo());
    if (isDS2){
        minFlow = std::min(minFlow, (isDS2->getDemand() - getTotVertex(sourceVertex, isDS2)));
    }

    while (currentVertex != sourceVertex){
        auto* isDS = dynamic_cast<DS*>(currentVertex->getInfo());
        if (isDS){
            minFlow = std::min(minFlow, (isDS->getDemand() - getTotVertex(currentVertex, isDS)));
        }

        auto e = currentVertex->getPath();
        //std::cout << "we are on vertex " << currentVertex->getInfo() << " the path is " << e->getWeight() << " and going to vertex " << e->getOrig()->getInfo() << "\n";
        if (e->getDest() == currentVertex) {
            minFlow = std::min(minFlow, e->getWeight() - e->getFlow());
            currentVertex = e->getOrig();
        } else {
            //std::cout << " The current edge is " << e->getWeight() << " the destination is " << e->getDest()->getInfo() << " and the source is" << e->getOrig()->getInfo() << "\n";
            minFlow = std::min(minFlow, e->getFlow());
            currentVertex = e->getDest();
        }

    }
    return minFlow;
}

/**
 * @brief Helper function.
 * @details Beginning from the target vertex we are going to follow the previously saved path
 * @details and add [In case the path belongs to the normal graph] or subtract
 * @details [In case we have found a path in the residual graph] the minimum residual capacity.
 * @details The time complexity for this function is O(V). This is because in the worst case we
 * @details need to go trough V vertexes (from the target to the source node).
 *
 * @param source - This is the ending node.
 * @param target - This is the starting node.
 * @param minFlow - Minimum residual capacity.
 **/
void WaterManager::augmentFlowAlongPath(WaterElement*& source, WaterElement*& target, double minFlow) {
    // Traverse the augmenting path and update the flow values accordingly
    Vertex<WaterElement*>* sourceVertex = waterNetwork.findVertex(source);

    Vertex<WaterElement*>* targetVertex = waterNetwork.findVertex(target);

    auto& currentVertex = targetVertex;

    // Note -> currentVertex starts at the target node and follows the path until it reaches the source node.
    while (currentVertex != sourceVertex){
        auto e = currentVertex->getPath();
        double flow = e->getFlow();
        auto* isDS = dynamic_cast<DS*>(currentVertex->getInfo());
        if (e->getDest() == currentVertex) {
            e->setFlow(flow + minFlow);

            if (isDS){
                isDS->setCurrentFlow(getTotVertex(currentVertex, isDS));
            }

            currentVertex = e->getOrig();
        } else {
            e->setFlow(flow - minFlow);

            if (isDS){
                isDS->setCurrentFlow(getTotVertex(currentVertex, isDS));
            }

            currentVertex = e->getDest();
        }
    }
}

std::string WaterManager::maximumFlowAllCities() {

    std::ostringstream oss;

    // Add superSource and superTarget
    WaterElement* superWaterReservoir = new WR("superWR", -1, "superWR", "none", 0);
    WaterElement* superDeliverySite = new DS("superDS",-1,"superDS",0,0,0);
    waterNetwork.addVertex(superWaterReservoir);
    waterNetwork.addVertex(superDeliverySite);

    // Calculate the max delivery for the superWaterReservoir.
    for (const auto& waterReservoir : waterReservoirMap){
        ((WR*)superWaterReservoir)->setMaxDelivery(((WR*)superWaterReservoir)->getMaxDelivery() + waterReservoir.second->getMaxDelivery());
        waterNetwork.addEdge(superWaterReservoir, waterReservoir.second, waterReservoir.second->getMaxDelivery());
    }

    // Calculate the demand for the superDeliverySite.
    for (const auto& city : waterCityMap){
        ((DS*)superDeliverySite)->setDemand(((DS*)superDeliverySite)->getDemand() + city.second->getDemand());
        waterNetwork.addEdge(city.second, superDeliverySite, city.second->getDemand());
    }

    // Set flow to 0.
    for (Vertex<WaterElement*>*& vertex : waterNetwork.getVertexSet()){
        for (Edge<WaterElement*>*& edge : vertex->getAdj()){
            edge->setFlow(0);
        }
    }

    while (existsAugmentingPath(superWaterReservoir, superDeliverySite)){
        double minFlow = findMinResidualAlongPath(superWaterReservoir, superDeliverySite);
        augmentFlowAlongPath(superWaterReservoir, superDeliverySite, minFlow);
    }

    for (const auto& city : waterCityMap){
        oss << "The city " << city.second->getCity() << " has a maximum flow of " << city.second->getCurrentFlow() << " cubic meters per second.\n";
    }

    for (Vertex<WaterElement*>* vertex : waterNetwork.getVertexSet()){
        for (Edge<WaterElement*>* edge : vertex->getIncoming()){
            if (edge->getDest()->getInfo()->getCode() == "superDS"){
                waterNetwork.removeEdge(edge->getOrig()->getInfo(), edge->getDest()->getInfo());
            }
        }
    }


    waterNetwork.removeVertex(superWaterReservoir);
    waterNetwork.removeVertex(superDeliverySite);
    return oss.str();
}

std::string WaterManager::maximumFlowSpecificCities(std::string cityCode) {

    std::ostringstream oss;

    auto wc = waterCityMap.find("c_" + cityCode);
    WaterElement* superWaterSource;

     for (auto vertex : waterNetwork.getVertexSet()){
         if (vertex->getInfo()->getCode() == wc->second->getCode()){
             superWaterSource = vertex->getInfo();
         }
     }

     if (superWaterSource == NULL){
         oss << "No city was found!\n";
         return oss.str();
     }

    // Add superSource and superTarget
    WaterElement* superWaterReservoir = new WR("superWR", -1, "superWR", "none", 0);
    waterNetwork.addVertex(superWaterReservoir);

    // Calculate the max delivery for the superWaterReservoir.
    for (const auto& waterReservoir : waterReservoirMap){
        ((WR*)superWaterReservoir)->setMaxDelivery(((WR*)superWaterReservoir)->getMaxDelivery() + waterReservoir.second->getMaxDelivery());
        waterNetwork.addEdge(superWaterReservoir, waterReservoir.second, waterReservoir.second->getMaxDelivery());
    }

    // Set flow to 0.
    for (Vertex<WaterElement*>*& vertex : waterNetwork.getVertexSet()){
        for (Edge<WaterElement*>*& edge : vertex->getAdj()){
            edge->setFlow(0);
        }
    }

    while (existsAugmentingPath(superWaterReservoir, superWaterSource)){
        double minFlow = findMinResidualAlongPath(superWaterReservoir, superWaterSource);
        augmentFlowAlongPath(superWaterReservoir, superWaterSource, minFlow);
    }

    oss << "The city " << ((DS*)superWaterSource)->getCity() << " has a maximum flow of " << ((DS*)superWaterSource)->getCurrentFlow() << " cubic meters per second.\n";

    waterNetwork.removeVertex(superWaterReservoir);
    return oss.str();
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

std::map<DS * , double> WaterManager::auxMaxFlow(){

    if(maximumFlowAllCities().empty()) std::cout << "ERROR! maxFlow failed.";

    std::map<DS * , double>cities;
    for (const auto& city : waterCityMap) {
        auto cityVertex = waterNetwork.findVertex(city.second);

        double tot = 0;
        for (Edge<WaterElement *> *incomingPipe: cityVertex->getIncoming()) {
            tot += incomingPipe->getFlow();
        }

        cities.insert({city.second,tot});

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

std::map<std::string, std::vector<std::pair<std::string, double>>> WaterManager::CitiesAffectedByPipeRupture(std::string &cityCode) {
    std::map<std::string, std::vector<std::pair<std::string, double>>> result;

    // Find the target city vertex
    Vertex<WaterElement*>* targetCityVertex = waterNetwork.findVertex(waterCityMap[cityCode]);


    // If the target city vertex is not found, return an empty result
    if (targetCityVertex == nullptr) {
        std::cout << "Error: City not found!\n";
        return result;
    }


    // Calculate original flows for all cities
    std::map<DS*,double> originalFlows = auxMaxFlow();


    for(Vertex<WaterElement*>* we : waterNetwork.getVertexSet()){
        // Loop through all the edges connected to the target city vertex
        for (Edge<WaterElement*>* pipe : we->getAdj()) {
            // Simulate pipe rupture by setting its flow capacity to 0
            auto destination = pipe->getDest()->getInfo();
            auto origin = pipe->getOrig()->getInfo();
            double weight = pipe->getWeight();
            waterNetwork.removeEdge(pipe->getOrig()->getInfo(), pipe->getDest()->getInfo());

            // Calculate the maximum flow after the pipe rupture
            std::map<DS *, double> maxFlows = auxMaxFlow();

            // Check if the desired water supply cannot be met for any city
            for (const auto &city: maxFlows) {

                if ((city.second < city.first->getDemand()) && (originalFlows[city.first] > city.second) && (targetCityVertex->getInfo()->getCode() == city.first->getCode())) {
                    // Record the affected city and the deficit in water supply
                    std::string affectedCity = city.first->getCity(); // Remove the prefix 'c_'
                    double deficit = city.first->getDemand() - city.second;
                    std::ostringstream oss;

                    oss << " from service point " << origin->getCode() << " to service point " << destination->getCode();
                    result[oss.str()].push_back(std::make_pair(affectedCity, deficit));
                }
            }
            // Restore the original flow capacity of the pipe

            waterNetwork.addEdge(origin, destination, weight);
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
    std::map<DS*,double> originalFlows = auxMaxFlow();

    for(Vertex<WaterElement*>* we : waterNetwork.getVertexSet()) {
        // Loop through all the edges connected to the target city vertex
        for (Edge<WaterElement *> *pipe: we->getAdj()) {
            // Simulate pipe rupture by setting its flow capacity to 0
            auto destination = pipe->getDest()->getInfo();
            auto origin = pipe->getOrig()->getInfo();
            double weight = pipe->getWeight();
            waterNetwork.removeEdge(pipe->getOrig()->getInfo(), pipe->getDest()->getInfo());

            // Calculate the maximum flow after the pipe rupture
            std::map<DS *, double> maxFlows = auxMaxFlow();

            // Check if the desired water supply cannot be met for any city
            for (const auto &city: maxFlows) {

                if ((city.second < city.first->getDemand()) && (originalFlows[city.first] > city.second)) {
                    // Record the affected city and the deficit in water supply
                    std::string affectedCity = city.first->getCity(); // Remove the prefix 'c_'
                    double deficit = city.first->getDemand() - city.second;
                    std::ostringstream oss;

                    oss << " from service point " << origin->getCode() << " to service point " << destination->getCode();
                    result[oss.str()].push_back(std::make_pair(affectedCity, deficit));
                }
            }

            // Restore the original flow capacity of the pipe

            waterNetwork.addEdge(origin, destination, weight);
        }
    }
    return result;
}




