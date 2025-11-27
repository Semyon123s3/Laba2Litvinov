#pragma once
#include "Pipe.h"
#include "CS.h"
#include "Logger.h"
#include <vector>
using namespace std;

class PipelineSystem {
private:
    vector<Pipe> pipes;
    vector<CS> stations;
    int nextPipeId;
    int nextCSId;
    Logger logger;

public:
    PipelineSystem();
    void showMenu();
    void run();
};