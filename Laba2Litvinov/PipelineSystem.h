#pragma once
#include "Pipe.h"
#include "CS.h"
#include "Logger.h"
#include <vector>
#include <string>
using namespace std;

class PipelineSystem {
private:
    vector<Pipe> pipes;
    vector<CS> stations;
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
    void showPipesList(const vector<Pipe*>& pipeList);
    void showCSList(const vector<CS*>& csList);

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
};