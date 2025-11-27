#pragma once
#include <string>
#include <iostream>
using namespace std;

class CS {
private:
    int id;
    string name;
    int totalWorkshops;
    int workingWorkshops;
    string efficiencyClass;

public:
    CS(int id = 0);

    int getId() const;
    string getName() const;
    int getTotalWorkshops() const;
    int getWorkingWorkshops() const;
    string getEfficiencyClass() const;
    double getUnusedPercentage() const;

    void setName(const string& name);
    void setTotalWorkshops(int total);
    void setWorkingWorkshops(int working);
    void setEfficiencyClass(const string& efficiencyClass);

    bool startWorkshop();
    bool stopWorkshop();
    void display() const;
};