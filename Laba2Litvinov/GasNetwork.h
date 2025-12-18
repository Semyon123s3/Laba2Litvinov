#pragma once
#include "Pipe.h"
#include "CS.h"
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

class GasNetwork {
private:
    unordered_map<int, Pipe>& pipes;
    unordered_map<int, CS>& stations;

    
    unordered_map<int, vector<int>> adjacencyList;  
    unordered_map<int, int> inDegree;               

public:
    GasNetwork(unordered_map<int, Pipe>& p, unordered_map<int, CS>& s);

    bool connectStations(int startId, int endId, int diameter);
    bool disconnectPipe(int pipeId);
    vector<int> topologicalSort();
    void showNetwork() const;

    bool connectOrCreatePipe(int startId, int endId, int diameter,
        unordered_map<int, Pipe>& pipes, int& nextPipeId);
    double calculateMaxFlow(int sourceId, int sinkId);
    void showMaxFlow();

private:
    int findAvailablePipe(int diameter) const;
    void updateGraph();
    bool hasCycle() const;
    void dfsCycleCheck(int v, unordered_map<int, bool>& visited,
        unordered_map<int, bool>& recStack, bool& cycleFound) const;
};