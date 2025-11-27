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

public:
    Pipe(int id = 0);

    int getId() const;
    string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isInRepair() const;

    void setName(const string& name);
    void setLength(double length);
    void setDiameter(int diameter);
    void setRepairStatus(bool status);

    void display() const;
    void toggleRepair();
};