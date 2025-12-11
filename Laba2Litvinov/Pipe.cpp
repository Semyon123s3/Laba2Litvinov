#include "Pipe.h"

Pipe::Pipe(int id) : id(id), length(0.0), diameter(0), inRepair(false),
isConnected(false), startStationId(-1), endStationId(-1) {
}

int Pipe::getId() const { return id; }
string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isInRepair() const { return inRepair; }

void Pipe::setName(const string& name) { this->name = name; }
void Pipe::setLength(double length) { if (length > 0) this->length = length; }
void Pipe::setDiameter(int diameter) { if (diameter > 0) this->diameter = diameter; }
void Pipe::setRepairStatus(bool status) { inRepair = status; }

void Pipe::display() const {
    cout << "ID: " << id << " | Название: " << name
        << " | Длина: " << length << " км | Диаметр: " << diameter
        << " мм | В ремонте: " << (inRepair ? "да" : "нет")
        << " | Соединена: " << (isConnected ?
            "да (КС" + to_string(startStationId) + " -> КС" + to_string(endStationId) + ")"
            : "нет") << endl;
}

void Pipe::toggleRepair() {
    inRepair = !inRepair;
}