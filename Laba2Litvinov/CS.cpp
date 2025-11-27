#include "CS.h"

CS::CS(int id) : id(id), totalWorkshops(0), workingWorkshops(0) {}

int CS::getId() const { return id; }
string CS::getName() const { return name; }
int CS::getTotalWorkshops() const { return totalWorkshops; }
int CS::getWorkingWorkshops() const { return workingWorkshops; }
string CS::getEfficiencyClass() const { return efficiencyClass; }

double CS::getUnusedPercentage() const {
    if (totalWorkshops == 0) return 0.0;
    return (1.0 - (double)workingWorkshops / totalWorkshops) * 100.0;
}

void CS::setName(const string& name) { this->name = name; }

void CS::setTotalWorkshops(int total) {
    if (total >= 0) {
        totalWorkshops = total;
        if (workingWorkshops > totalWorkshops) {
            workingWorkshops = totalWorkshops;
        }
    }
}

void CS::setWorkingWorkshops(int working) {
    if (working >= 0 && working <= totalWorkshops) {
        workingWorkshops = working;
    }
}

void CS::setEfficiencyClass(const string& efficiencyClass) {
    this->efficiencyClass = efficiencyClass;
}

bool CS::startWorkshop() {
    if (workingWorkshops < totalWorkshops) {
        workingWorkshops++;
        return true;
    }
    return false;
}

bool CS::stopWorkshop() {
    if (workingWorkshops > 0) {
        workingWorkshops--;
        return true;
    }
    return false;
}

void CS::display() const {
    cout << "ID: " << id << " | Название: " << name
        << " | Цеха: " << workingWorkshops << "/" << totalWorkshops
        << " | Незадействовано: " << getUnusedPercentage() << "%"
        << " | Класс: " << efficiencyClass << endl;
}