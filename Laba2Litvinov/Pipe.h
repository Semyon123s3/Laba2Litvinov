#pragma once
#include <string>
#include <iostream>
using namespace std;

class Pipe {
private:
    int id;
    string name;
    double length;
    int diameter;
    bool inRepair;

    bool isConnected;          
    int startStationId;        
    int endStationId;          

public:
    Pipe(int id = 0);

    int getId() const;
    string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isInRepair() const;

    bool getIsConnected() const { return isConnected; }
    int getStartStationId() const { return startStationId; }
    int getEndStationId() const { return endStationId; }

    void setName(const string& name);
    void setLength(double length);
    void setDiameter(int diameter);
    void setRepairStatus(bool status);

    void display() const;
    void toggleRepair();

    void connect(int startId, int endId) {
        isConnected = true;
        startStationId = startId;
        endStationId = endId;
    }

    void disconnect() {
        isConnected = false;
        startStationId = -1;   
        endStationId = -1;
    }

    bool isConnectedToStation(int stationId) const {
        return isConnected && (startStationId == stationId || endStationId == stationId);
    }
};