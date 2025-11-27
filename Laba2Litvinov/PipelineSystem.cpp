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

void PipelineSystem::addPipe() {
    cout << "\n=== ДОБАВЛЕНИЕ ТРУБЫ ===" << endl;

    Pipe newPipe(nextPipeId++);

    newPipe.setName(inputString("Введите километровую отметку (название): "));
    newPipe.setLength(inputDouble("Введите длину трубы (км): ", 0.1, 10000.0));
    newPipe.setDiameter(inputInt("Введите диаметр трубы (мм): ", 1, 5000));

    int repairStatus = inputInt("В ремонте (1-да, 0-нет): ", 0, 1);
    newPipe.setRepairStatus(repairStatus == 1);

    pipes.push_back(newPipe);
    logger.log("Добавлена труба ID: " + to_string(newPipe.getId()) + ", название: " + newPipe.getName());
    cout << "Труба успешно добавлена! ID: " << newPipe.getId() << endl;
}

void PipelineSystem::addCS() {
    cout << "\n=== ДОБАВЛЕНИЕ КОМПРЕССОРНОЙ СТАНЦИИ ===" << endl;

    CS newCS(nextCSId++);

    newCS.setName(inputString("Введите название КС: "));
    newCS.setTotalWorkshops(inputInt("Введите общее количество цехов: ", 1, 1000));
    newCS.setWorkingWorkshops(inputInt("Введите количество работающих цехов: ", 0, newCS.getTotalWorkshops()));
    newCS.setEfficiencyClass(inputString("Введите класс эффективности: "));

    stations.push_back(newCS);
    logger.log("Добавлена КС ID: " + to_string(newCS.getId()) + ", название: " + newCS.getName());
    cout << "КС успешно добавлена! ID: " << newCS.getId() << endl;
}

void PipelineSystem::showAllObjects() {
    cout << "\n=== ВСЕ ОБЪЕКТЫ ===" << endl;

    cout << "\n--- ТРУБЫ (" << pipes.size() << ") ---" << endl;
    if (pipes.empty()) {
        cout << "Трубы не добавлены." << endl;
    }
    else {
        for (const auto& pipe : pipes) {
            pipe.display();
        }
    }

    cout << "\n--- КОМПРЕССОРНЫЕ СТАНЦИИ (" << stations.size() << ") ---" << endl;
    if (stations.empty()) {
        cout << "КС не добавлены." << endl;
    }
    else {
        for (const auto& cs : stations) {
            cs.display();
        }
    }

    logger.log("Просмотр всех объектов");
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
        if (!(cin >> choice) || cin.peek() != '\n') {
            cout << "Ошибка ввода! Пожалуйста, введите число." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            addPipe();
            break;
        case 2:
            addCS();
            break;
        case 3:
            showAllObjects();
            break;
        case 0:
            cout << "Выход из программы." << endl;
            return;
        default:
            cout << "Неверный выбор! Пожалуйста, выберите действие из меню." << endl;
        }

        cout << "\nНажмите Enter для продолжения...";
        cin.get();
    }
}