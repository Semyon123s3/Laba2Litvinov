#include "GasNetwork.h"
#include <iostream>
#include <algorithm>
#include "Constants.h"
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



