#include "PipelineSystem.h"
#include <iostream>
#include <limits>
#include <algorithm>
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

Pipe* PipelineSystem::findPipeById(int id) {
    for (auto& pipe : pipes) {
        if (pipe.getId() == id) {
            return &pipe;
        }
    }
    return nullptr;
}

CS* PipelineSystem::findCSById(int id) {
    for (auto& cs : stations) {
        if (cs.getId() == id) {
            return &cs;
        }
    }
    return nullptr;
}

void PipelineSystem::showAllPipes() {
    cout << "\n--- ВСЕ ТРУБЫ ---" << endl;
    if (pipes.empty()) {
        cout << "Трубы не добавлены." << endl;
    }
    else {
        for (const auto& pipe : pipes) {
            pipe.display();
        }
    }
}

void PipelineSystem::showAllCS() {
    cout << "\n--- ВСЕ КС ---" << endl;
    if (stations.empty()) {
        cout << "КС не добавлены." << endl;
    }
    else {
        for (const auto& cs : stations) {
            cs.display();
        }
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

void PipelineSystem::editPipe() {
    if (pipes.empty()) {
        cout << "Нет доступных труб для редактирования." << endl;
        return;
    }

    showAllPipes();
    int id = inputInt("Введите ID трубы для редактирования: ", 1, nextPipeId - 1);

    Pipe* pipe = findPipeById(id);
    if (!pipe) {
        cout << "Труба с ID " << id << " не найдена." << endl;
        return;
    }

    cout << "\nРедактирование трубы ID: " << pipe->getId() << endl;
    pipe->setName(inputString("Новое название: "));
    pipe->setLength(inputDouble("Новая длина (км): ", 0.1, 10000.0));
    pipe->setDiameter(inputInt("Новый диаметр (мм): ", 1, 5000));

    int repairStatus = inputInt("В ремонте (1-да, 0-нет): ", 0, 1);
    pipe->setRepairStatus(repairStatus == 1);

    logger.log("Отредактирована труба ID: " + to_string(pipe->getId()));
    cout << "Труба успешно отредактирована!" << endl;
}

void PipelineSystem::editCS() {
    if (stations.empty()) {
        cout << "Нет доступных КС для редактирования." << endl;
        return;
    }

    showAllCS();
    int id = inputInt("Введите ID КС для редактирования: ", 1, nextCSId - 1);

    CS* cs = findCSById(id);
    if (!cs) {
        cout << "КС с ID " << id << " не найдена." << endl;
        return;
    }

    cout << "\nРедактирование КС ID: " << cs->getId() << endl;
    cs->setName(inputString("Новое название: "));
    cs->setTotalWorkshops(inputInt("Общее количество цехов: ", 1, 1000));
    cs->setWorkingWorkshops(inputInt("Количество работающих цехов: ", 0, cs->getTotalWorkshops()));
    cs->setEfficiencyClass(inputString("Класс эффективности: "));

    logger.log("Отредактирована КС ID: " + to_string(cs->getId()));
    cout << "КС успешно отредактирована!" << endl;
}

void PipelineSystem::deletePipe() {
    if (pipes.empty()) {
        cout << "Нет доступных труб для удаления." << endl;
        return;
    }

    showAllPipes();
    int id = inputInt("Введите ID трубы для удаления: ", 1, nextPipeId - 1);

    auto it = remove_if(pipes.begin(), pipes.end(),
        [id](const Pipe& pipe) { return pipe.getId() == id; });

    if (it != pipes.end()) {
        pipes.erase(it, pipes.end());
        logger.log("Удалена труба ID: " + to_string(id));
        cout << "Труба успешно удалена!" << endl;
    }
    else {
        cout << "Труба с ID " << id << " не найдена." << endl;
    }
}

void PipelineSystem::deleteCS() {
    if (stations.empty()) {
        cout << "Нет доступных КС для удаления." << endl;
        return;
    }

    showAllCS();
    int id = inputInt("Введите ID КС для удаления: ", 1, nextCSId - 1);

    auto it = remove_if(stations.begin(), stations.end(),
        [id](const CS& cs) { return cs.getId() == id; });

    if (it != stations.end()) {
        stations.erase(it, stations.end());
        logger.log("Удалена КС ID: " + to_string(id));
        cout << "КС успешно удалена!" << endl;
    }
    else {
        cout << "КС с ID " << id << " не найдена." << endl;
    }
}

void PipelineSystem::showMenu() {
    cout << "\n=== СИСТЕМА УПРАВЛЕНИЯ ТРУБОПРОВОДОМ ===" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Удалить трубу" << endl;
    cout << "7. Удалить КС" << endl;
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
        case 4:
            editPipe();
            break;
        case 5:
            editCS();
            break;
        case 6:
            deletePipe();
            break;
        case 7:
            deleteCS();
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