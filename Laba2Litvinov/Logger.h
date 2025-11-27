#pragma once
#include <string>
#include <fstream>
using namespace std;

class Logger {
private:
    ofstream logFile;

public:
    Logger();
    ~Logger();

    void log(const string& message);
};