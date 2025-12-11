#pragma once
#include "Pipe.h"
#include "CS.h"
#include "Logger.h"
#include <unordered_map>
#include <string>
using namespace std;

class PipelineSystem {
private:
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> stations;
    int nextPipeId;
    int nextCSId;
    Logger logger;

    string inputString(const string& prompt);
    int inputInt(const string& prompt, int min, int max);
    double inputDouble(const string& prompt, double min, double max);

    Pipe* findPipeById(int id);
    CS* findCSById(int id);
    void showAllPipes();
    void showAllCS();
    void showPipesList(const unordered_map<int, Pipe*>& pipeMap);
    void showCSList(const unordered_map<int, CS*>&csMap);

public:
    PipelineSystem();
    void showMenu();
    void run();

    void addPipe();
    void addCS();
    void showAllObjects();
    void editPipe();
    void editCS();
    void deletePipe();
    void deleteCS();
    void searchPipes();
    void searchCS();
    void batchEditPipes();
    void saveToFile();
    void loadFromFile();
};