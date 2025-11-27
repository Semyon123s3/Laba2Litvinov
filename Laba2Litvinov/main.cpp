#include <iostream>
#include <windows.h>
#include "PipelineSystem.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    PipelineSystem system;
    system.run();
    return 0;
}