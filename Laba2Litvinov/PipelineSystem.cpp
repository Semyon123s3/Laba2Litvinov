#include "PipelineSystem.h"
#include <iostream>
#include <limits>
using namespace std;

PipelineSystem::PipelineSystem() : nextPipeId(1), nextCSId(1) {}

string PipelineSystem::inputString(const string& prompt) {
    string result;
    while (true) {
        cout << prompt;
        getline(cin, result);
        if (!result.empty()) {
            return result;
        }
        cout << "Ошибка! Поле не может быть пустым. Попробуйте снова." << endl;
    }
}

int PipelineSystem::inputInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Ошибка! Введите целое число от " << min << " до " << max << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double PipelineSystem::inputDouble(const string& prompt, double min, double max) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void PipelineSystem::showMenu() {
    cout << "\n=== СИСТЕМА УПРАВЛЕНИЯ ТРУБОПРОВОДОМ ===" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

void PipelineSystem::run() {
    int choice;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cout << "Ошибка ввода! Пожалуйста, введите число." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            cout << "Добавление трубы - В РАЗРАБОТКЕ" << endl;
            break;
        case 2:
            cout << "Добавление КС - В РАЗРАБОТКЕ" << endl;
            break;
        case 3:
            cout << "Просмотр объектов - В РАЗРАБОТКЕ" << endl;
            break;
        case 0:
            cout << "Выход из программы." << endl;
            return;
        default:
            cout << "Неверный выбор!" << endl;
        }

        cout << "\nНажмите Enter для продолжения...";
        cin.get();
    }
}