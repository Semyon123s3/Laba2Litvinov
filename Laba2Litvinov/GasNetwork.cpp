#include "GasNetwork.h"
#include <iostream>
#include <algorithm>
#include "Constants.h"
#include <set>
using namespace std;

GasNetwork::GasNetwork(unordered_map<int, Pipe>& p, unordered_map<int, CS>& s)
    : pipes(p), stations(s) {
    updateGraph();   
}

void GasNetwork::updateGraph() {
    adjacencyList.clear();
    inDegree.clear();

    for (const auto& pair : stations) {
        int id = pair.first;
        adjacencyList[id] = vector<int>();   
        inDegree[id] = 0;                    
    }

    for (const auto& pair : pipes) {
        const Pipe& pipe = pair.second;
        if (pipe.getIsConnected()) {   
            int start = pipe.getStartStationId();
            int end = pipe.getEndStationId();
            adjacencyList[start].push_back(end);   
            inDegree[end]++;                       
        }
    }
}


int GasNetwork::findAvailablePipe(int diameter) const {
    for (const auto& pair : pipes) {
        const Pipe& pipe = pair.second;
        if (!pipe.getIsConnected() && pipe.getDiameter() == diameter) {
            return pipe.getId();   
        }
    }
    return -1;   
}


bool GasNetwork::connectStations(int startId, int endId, int diameter) {
    if (stations.find(startId) == stations.end() ||
        stations.find(endId) == stations.end()) {
        cout << "Ошибка: одна из КС не найдена!" << endl;
        return false;
    }

    if (startId == endId) {
        cout << "Ошибка: нельзя соединить КС саму с собой!" << endl;
        return false;
    }

    int pipeId = findAvailablePipe(diameter);
    if (pipeId == -1) {
        cout << "Нет свободных труб диаметром " << diameter << " мм." << endl;
        return false;
    }

    Pipe& pipe = pipes[pipeId];
    pipe.connect(startId, endId);

    cout << "Соединение создано: КС" << startId << " -> КС" << endId
        << " через трубу ID " << pipeId << endl;

    updateGraph();   
    return true;
}

bool GasNetwork::disconnectPipe(int pipeId) {
    if (pipes.find(pipeId) == pipes.end()) {
        cout << "Труба с ID " << pipeId << " не найдена." << endl;
        return false;
    }

    Pipe& pipe = pipes[pipeId];
    if (!pipe.getIsConnected()) {
        cout << "Труба с ID " << pipeId << " не соединена." << endl;
        return false;
    }

    pipe.disconnect();   
    updateGraph();       

    cout << "Соединение через трубу ID " << pipeId << " разорвано." << endl;
    return true;
}

void GasNetwork::dfsCycleCheck(int v, unordered_map<int, bool>& visited,
    unordered_map<int, bool>& recStack, bool& cycleFound) const {
    if (cycleFound) return; 

    visited[v] = true;
    recStack[v] = true;

    for (int neighbor : adjacencyList.at(v)) {
        if (!visited[neighbor]) {
            dfsCycleCheck(neighbor, visited, recStack, cycleFound);
        }
        else if (recStack[neighbor]) {
            cycleFound = true;   
            return;
        }
    }

    recStack[v] = false;   
}

bool GasNetwork::hasCycle() const {
    unordered_map<int, bool> visited;
    unordered_map<int, bool> recStack;   
    bool cycleFound = false;

    for (const auto& pair : stations) {
        visited[pair.first] = false;
        recStack[pair.first] = false;
    }

    for (const auto& pair : stations) {
        if (!visited[pair.first]) {
            dfsCycleCheck(pair.first, visited, recStack, cycleFound);
            if (cycleFound) break;
        }
    }

    return cycleFound;
}

vector<int> GasNetwork::topologicalSort() {
    vector<int> result;

    if (hasCycle()) {
        cout << "Обнаружен цикл! Топологическая сортировка невозможна." << endl;
        return result; 
    }

    unordered_map<int, int> indegree = inDegree;
    queue<int> q;   

    for (const auto& pair : stations) {
        if (indegree[pair.first] == 0) {
            q.push(pair.first);
        }
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);  

        for (int neighbor : adjacencyList[current]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if (result.size() != stations.size()) {
        cout << "Ошибка при топологической сортировке." << endl;
        result.clear();  
    }

    return result;
}

void GasNetwork::showNetwork() const {
    cout << "\n=== ГАЗОТРАНСПОРТНАЯ СЕТЬ ===" << endl;

    if (pipes.empty()) {
        cout << "Нет труб." << endl;
        return;
    }

    bool hasConnections = false;
    for (const auto& pair : pipes) {
        const Pipe& pipe = pair.second;
        if (pipe.getIsConnected()) {
            hasConnections = true;
            cout << "КС" << pipe.getStartStationId() << " -- Труба ID " << pipe.getId()
                << " (диаметр " << pipe.getDiameter() << " мм, длина " << pipe.getLength()
                << " км) --> КС" << pipe.getEndStationId() << endl;
        }
    }

    if (!hasConnections) {
        cout << "Нет соединений." << endl;
    }
}

bool GasNetwork::connectOrCreatePipe(int startId, int endId, int diameter,
    unordered_map<int, Pipe>& pipes, int& nextPipeId) {
    if (stations.find(startId) == stations.end() ||
        stations.find(endId) == stations.end()) {
        cout << "Ошибка: одна из КС не найдена!" << endl;
        return false;
    }

    if (startId == endId) {
        cout << "Ошибка: нельзя соединить КС саму с собой!" << endl;
        return false;
    }

    int pipeId = findAvailablePipe(diameter);

    if (pipeId == -1) {
        cout << "\n--- СОЗДАНИЕ НОВОЙ ТРУБЫ ---" << endl;
        cout << "Нет свободных труб диаметром " << diameter << " мм." << endl;

        pipeId = nextPipeId++;

        cout << "Введите параметры для новой трубы:" << endl;

        Pipe newPipe(pipeId);
        newPipe.setDiameter(diameter);
        newPipe.setLength(10.0);  
        newPipe.setName("Труба для соединения КС" + to_string(startId) + "-КС" + to_string(endId));

        pipes[pipeId] = newPipe;
        cout << "Создана новая труба ID: " << pipeId << endl;
    }

    Pipe& pipe = pipes[pipeId];
    pipe.connect(startId, endId);

    cout << "Соединение: КС" << startId << " -> КС" << endId
        << " через трубу ID " << pipeId << " (диаметр: " << diameter << " мм)" << endl;

    updateGraph();
    return true;
}

double GasNetwork::calculateMaxFlow(int sourceId, int sinkId) {
    if (stations.find(sourceId) == stations.end() ||
        stations.find(sinkId) == stations.end()) {
        return 0.0;
    }

    if (sourceId == sinkId) {
        return numeric_limits<double>::infinity();
    }

    unordered_map<int, vector<pair<int, double>>> graph;

    for (const auto& station : stations) {
        graph[station.first] = vector<pair<int, double>>();
    }

    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;
        if (pipe.getIsConnected()) {
            int from = pipe.getStartStationId();
            int to = pipe.getEndStationId();
            double capacity = pipe.getCapacity();

            graph[from].push_back({ to, capacity });
        }
    }

    double maxFlow = 0.0;

    while (true) {
        queue<int> q;
        unordered_map<int, int> parent;
        unordered_map<int, double> capacityTo;

        for (const auto& station : stations) {
            parent[station.first] = -1;
        }

        q.push(sourceId);
        parent[sourceId] = sourceId;
        capacityTo[sourceId] = numeric_limits<double>::infinity();

        bool foundPath = false;

        while (!q.empty() && !foundPath) {
            int current = q.front();
            q.pop();

            for (const auto& edge : graph[current]) {
                int neighbor = edge.first;
                double capacity = edge.second;

                if (parent[neighbor] == -1 && capacity > 1e-9) {
                    parent[neighbor] = current;
                    capacityTo[neighbor] = min(capacityTo[current], capacity);

                    if (neighbor == sinkId) {
                        foundPath = true;
                        break;
                    }

                    q.push(neighbor);
                }
            }
        }

        if (!foundPath) break;

        double pathFlow = capacityTo[sinkId];
        maxFlow += pathFlow;

        int current = sinkId;
        while (current != sourceId) {
            int prev = parent[current];

            for (auto& edge : graph[prev]) {
                if (edge.first == current) {
                    edge.second -= pathFlow;
                    break;
                }
            }

            bool hasReverseEdge = false;
            for (auto& edge : graph[current]) {
                if (edge.first == prev) {
                    edge.second += pathFlow;
                    hasReverseEdge = true;
                    break;
                }
            }

            if (!hasReverseEdge) {
                graph[current].push_back({ prev, pathFlow });
            }

            current = prev;
        }
    }

    return maxFlow;
}

void GasNetwork::showMaxFlow() {
    cout << "\n=== РАСЧЕТ МАКСИМАЛЬНОГО ПОТОКА ===" << endl;

    if (stations.size() < 2) {
        cout << "Для расчета нужно минимум 2 КС!" << endl;
        return;
    }

    cout << "\nДоступные компрессорные станции:" << endl;
    for (const auto& station : stations) {
        cout << "  КС" << station.first << " - " << station.second.getName() << endl;
    }


    int sourceId, sinkId;
    cout << "\nВведите ID КС-источника: ";
    cin >> sourceId;
    cout << "Введите ID КС-стока: ";
    cin >> sinkId;

    if (stations.find(sourceId) == stations.end()) {
        cout << "Ошибка: КС" << sourceId << " не существует!" << endl;
        return;
    }
    if (stations.find(sinkId) == stations.end()) {
        cout << "Ошибка: КС" << sinkId << " не существует!" << endl;
        return;
    }

    double maxFlow = calculateMaxFlow(sourceId, sinkId);

    cout << "\n═══════════════════════════════════════════" << endl;
    cout << "РЕЗУЛЬТАТЫ РАСЧЕТА:" << endl;
    cout << "═══════════════════════════════════════════" << endl;
    cout << "Максимальный поток от КС" << sourceId
        << " к КС" << sinkId << ":" << endl;
    cout << "  " << maxFlow << " млн м³/день" << endl;

    if (maxFlow == 0) {
        cout << "\n╔═════════════════════════════════════════╗" << endl;
        cout << "║ ВОЗМОЖНЫЕ ПРИЧИНЫ НУЛЕВОГО ПОТОКА:      ║" << endl;
        cout << "╠═════════════════════════════════════════╣" << endl;
        cout << "║ 1. Нет соединений между этими КС       ║" << endl;
        cout << "║ 2. Все трубы на пути в ремонте         ║" << endl;
        cout << "║ 3. Диаметр труб слишком мал            ║" << endl;
        cout << "╚═════════════════════════════════════════╝" << endl;
    }
    else if (maxFlow < 1.0) {
        cout << "Примечание: поток очень мал (возможно, есть трубы в ремонте)" << endl;
    }
    cout << "═══════════════════════════════════════════" << endl;
}

pair<double, vector<int>> GasNetwork::calculateShortestPath(int startId, int endId) {
    if (stations.find(startId) == stations.end() ||
        stations.find(endId) == stations.end()) {
        return { INFINITY_WEIGHT, {} };
    }

    unordered_map<int, double> dist;      
    unordered_map<int, int> prev;         
    set<pair<double, int>> priorityQueue; 

    for (const auto& station : stations) {
        dist[station.first] = INFINITY_WEIGHT;
        prev[station.first] = -1;
    }

    dist[startId] = 0;
    priorityQueue.insert({ 0, startId });

    while (!priorityQueue.empty()) {
        auto it = priorityQueue.begin();
        double currentDist = it->first;
        int u = it->second;
        priorityQueue.erase(it);

        if (u == endId) break;

        for (const auto& pipePair : pipes) {
            const Pipe& pipe = pipePair.second;

            if (pipe.getIsConnected() && pipe.getStartStationId() == u) {
                int v = pipe.getEndStationId();  
                double weight = pipe.getWeightForPath();  

                double newDist = currentDist + weight;

                if (newDist < dist[v]) {
                    auto oldPair = priorityQueue.find({ dist[v], v });
                    if (oldPair != priorityQueue.end()) {
                        priorityQueue.erase(oldPair);
                    }

                    dist[v] = newDist;
                    prev[v] = u;

                    priorityQueue.insert({ newDist, v });
                }
            }
        }
    }

    vector<int> path;
    if (dist[endId] < INFINITY_WEIGHT) {
        for (int at = endId; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
    }

    return { dist[endId], path };
}

void GasNetwork::showShortestPath() {
    cout << "\n=== ПОИСК КРАТЧАЙШЕГО ПУТИ ===" << endl;

    if (stations.size() < 2) {
        cout << "Для поиска нужно минимум 2 КС!" << endl;
        return;
    }

    cout << "\nДоступные компрессорные станции:" << endl;
    for (const auto& station : stations) {
        cout << "  КС" << station.first << " - " << station.second.getName() << endl;
    }

    int startId, endId;
    cout << "\nВведите ID начальной КС: ";
    cin >> startId;
    cout << "Введите ID конечной КС: ";
    cin >> endId;

    if (stations.find(startId) == stations.end()) {
        cout << "Ошибка: КС" << startId << " не существует!" << endl;
        return;
    }
    if (stations.find(endId) == stations.end()) {
        cout << "Ошибка: КС" << endId << " не существует!" << endl;
        return;
    }

    auto result = calculateShortestPath(startId, endId);
    double distance = result.first;
    vector<int> path = result.second;

    cout << "\n═══════════════════════════════════════════" << endl;
    cout << "РЕЗУЛЬТАТЫ ПОИСКА:" << endl;
    cout << "═══════════════════════════════════════════" << endl;

    if (path.empty()) {
        cout << "Путь от КС" << startId << " к КС" << endId << " не найден!" << endl;
        cout << "\nВозможные причины:" << endl;
        cout << "1. Нет соединений между этими КС" << endl;
        cout << "2. Все трубы на пути в ремонте" << endl;
        cout << "3. Граф несвязный" << endl;
    }
    else {
        cout << "Кратчайший путь от КС" << startId << " к КС" << endId << ":" << endl;
        cout << "  Общая длина: " << distance << " км" << endl;
        cout << "  Маршрут: ";

        for (size_t i = 0; i < path.size(); ++i) {
            cout << "КС" << path[i];
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;


        cout << "\nИнформация о пути:" << endl;
        cout << "  Количество переходов: " << path.size() - 1 << endl;

        if (distance >= INFINITY_WEIGHT * 0.9) {
            cout << "  ВНИМАНИЕ: На пути есть трубы в ремонте!" << endl;
        }
    }
    cout << "═══════════════════════════════════════════" << endl;
}


