#include "Logger.h"
#include <iostream>

Logger::Logger() {
    logFile.open("pipeline_log.txt", ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const string& message) {
    if (logFile.is_open()) {
        logFile << message << endl;
    }
    cout << "[кнц] " << message << endl;
}