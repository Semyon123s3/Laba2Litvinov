#include "PipelineSystem.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <vector>
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

    cout << "Выберите что редактировать:" << endl;
    cout << "1. Основные параметры" << endl;
    cout << "2. Переключить статус ремонта" << endl;

    int editType = inputInt("Ваш выбор: ", 1, 2);

    if (editType == 1) {
        pipe->setName(inputString("Новое название: "));
        pipe->setLength(inputDouble("Новая длина (км): ", 0.1, 10000.0));
        pipe->setDiameter(inputInt("Новый диаметр (мм): ", 1, 5000));

        int repairStatus = inputInt("В ремонте (1-да, 0-нет): ", 0, 1);
        pipe->setRepairStatus(repairStatus == 1);
    }
    else {
        pipe->toggleRepair();
        cout << "Статус ремонта изменен. Теперь: "
            << (pipe->isInRepair() ? "в ремонте" : "не в ремонте") << endl;
    }

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

    cout << "Выберите что редактировать:" << endl;
    cout << "1. Основные параметры" << endl;
    cout << "2. Управление цехами" << endl;

    int editType = inputInt("Ваш выбор: ", 1, 2);

    if (editType == 1) {
        cs->setName(inputString("Новое название: "));
        cs->setTotalWorkshops(inputInt("Общее количество цехов: ", 1, 1000));
        cs->setWorkingWorkshops(inputInt("Количество работающих цехов: ", 0, cs->getTotalWorkshops()));
        cs->setEfficiencyClass(inputString("Класс эффективности: "));
    }
    else {
        cout << "\nТекущее состояние: " << cs->getWorkingWorkshops() << " из " << cs->getTotalWorkshops() << " цехов работают" << endl;
        cout << "1. Запустить цех" << endl;
        cout << "2. Остановить цех" << endl;
        cout << "0. Отмена" << endl;

        int action = inputInt("Выберите действие: ", 0, 2);

        switch (action) {
        case 1:
            if (cs->startWorkshop()) {
                cout << "Цех запущен! Теперь работает " << cs->getWorkingWorkshops()
                    << " из " << cs->getTotalWorkshops() << " цехов." << endl;
            }
            else {
                cout << "Ошибка: Все цехи уже работают!" << endl;
            }
            break;
        case 2:
            if (cs->stopWorkshop()) {
                cout << "Цех остановлен! Теперь работает " << cs->getWorkingWorkshops()
                    << " из " << cs->getTotalWorkshops() << " цехов." << endl;
            }
            else {
                cout << "Ошибка: Нет работающих цехов!" << endl;
            }
            break;
        case 0:
            cout << "Операция отменена." << endl;
            break;
        }
    }

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

void PipelineSystem::searchPipes() {
    cout << "\n=== ПОИСК ТРУБ ===" << endl;
    cout << "1. По названию" << endl;
    cout << "2. По признаку 'в ремонте'" << endl;
    cout << "3. Все трубы" << endl;

    int choice = inputInt("Выберите критерий поиска: ", 1, 3);
    vector<Pipe*> foundPipes;

    switch (choice) {
    case 1: {
        string name = inputString("Введите название для поиска: ");
        for (auto& pipe : pipes) {
            if (pipe.getName().find(name) != string::npos) {
                foundPipes.push_back(&pipe);
            }
        }
        break;
    }
    case 2: {
        bool status = inputInt("В ремонте (1-да, 0-нет): ", 0, 1) == 1;
        for (auto& pipe : pipes) {
            if (pipe.isInRepair() == status) {
                foundPipes.push_back(&pipe);
            }
        }
        break;
    }
    case 3:
        for (auto& pipe : pipes) {
            foundPipes.push_back(&pipe);
        }
        break;
    }

    showPipesList(foundPipes);
    logger.log("Поиск труб: найдено " + to_string(foundPipes.size()) + " объектов");
}

void PipelineSystem::searchCS() {
    cout << "\n=== ПОИСК КС ===" << endl;
    cout << "1. По названию" << endl;
    cout << "2. По проценту незадействованных цехов" << endl;
    cout << "3. Все КС" << endl;

    int choice = inputInt("Выберите критерий поиска: ", 1, 3);
    vector<CS*> foundCS;

    switch (choice) {
    case 1: {
        string name = inputString("Введите название для поиска: ");
        for (auto& cs : stations) {
            if (cs.getName().find(name) != string::npos) {
                foundCS.push_back(&cs);
            }
        }
        break;
    }
    case 2: {
        double minPercent = inputDouble("Минимальный процент незадействованных цехов: ", 0, 100);
        double maxPercent = inputDouble("Максимальный процент незадействованных цехов: ", minPercent, 100);

        for (auto& cs : stations) {
            double percent = cs.getUnusedPercentage();
            if (percent >= minPercent && percent <= maxPercent) {
                foundCS.push_back(&cs);
            }
        }
        break;
    }
    case 3:
        for (auto& cs : stations) {
            foundCS.push_back(&cs);
        }
        break;
    }

    showCSList(foundCS);
    logger.log("Поиск КС: найдено " + to_string(foundCS.size()) + " объектов");
}

void PipelineSystem::showPipesList(const vector<Pipe*>& pipeList) {
    cout << "\n--- НАЙДЕННЫЕ ТРУБЫ (" << pipeList.size() << ") ---" << endl;
    if (pipeList.empty()) {
        cout << "Трубы не найдены." << endl;
    }
    else {
        for (const auto& pipe : pipeList) {
            pipe->display();
        }
    }
}

void PipelineSystem::showCSList(const vector<CS*>& csList) {
    cout << "\n--- НАЙДЕННЫЕ КС (" << csList.size() << ") ---" << endl;
    if (csList.empty()) {
        cout << "КС не найдены." << endl;
    }
    else {
        for (const auto& cs : csList) {
            cs->display();
        }
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
    cout << "8. Поиск труб" << endl;
    cout << "9. Поиск КС" << endl;
    cout << "10. Пакетное редактирование труб" << endl;
    cout << "11. Сохранить в файл" << endl;
    cout << "12. Загрузить из файла" << endl;
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
        case 8:
            searchPipes();
            break;
        case 9:
            searchCS();
            break;
        case 10:
            batchEditPipes();
            break;
        case 11:
            saveToFile();
            break;
        case 12:
            loadFromFile();
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

void PipelineSystem::batchEditPipes() {
    if (pipes.empty()) {
        cout << "Нет доступных труб для редактирования." << endl;
        return;
    }
    cout << "\n=== ПАКЕТНОЕ РЕДАКТИРОВАНИЕ ТРУБ ===" << endl;

    if (pipes.empty()) {
        cout << "Нет доступных труб для редактирования." << endl;
        return;
    }

    vector<Pipe*> pipesToEdit;

    cout << "Выберите трубы для редактирования:" << endl;
    searchPipes();  

    string input;
    cout << "Введите ID труб через пробел для редактирования (или 'all' для всех найденных): ";
    getline(cin, input);

    if (input == "all") {
        for (auto& pipe : pipes) {
            pipesToEdit.push_back(&pipe);
        }
    }
    else {
        stringstream ss(input);
        int id;
        while (ss >> id) {
            Pipe* pipe = findPipeById(id);
            if (pipe) {
                pipesToEdit.push_back(pipe);
            }
        }
    }

    if (pipesToEdit.empty()) {
        cout << "Не выбрано ни одной трубы для редактирования." << endl;
        return;
    }

    cout << "\nВыбрано труб для редактирования: " << pipesToEdit.size() << endl;

    cout << "\nВыберите действие:" << endl;
    cout << "1. Изменить статус ремонта" << endl;
    cout << "2. Удалить выбранные трубы" << endl;
    cout << "0. Отмена" << endl;

    int action = inputInt("Ваш выбор: ", 0, 2);

    switch (action) {
    case 1: {
        bool newStatus = inputInt("Новый статус (1-в ремонте, 0-работает): ", 0, 1) == 1;
        for (auto pipe : pipesToEdit) {
            pipe->setRepairStatus(newStatus);
        }
        logger.log("Пакетное изменение статуса труб: " + to_string(pipesToEdit.size()) + " объектов");
        cout << "Статус " << pipesToEdit.size() << " труб изменен!" << endl;
        break;
    }
    case 2: {
        for (auto pipe : pipesToEdit) {
            pipes.erase(remove_if(pipes.begin(), pipes.end(),
                [pipe](const Pipe& p) { return p.getId() == pipe->getId(); }),
                pipes.end());
        }
        logger.log("Пакетное удаление труб: " + to_string(pipesToEdit.size()) + " объектов");
        cout << "Удалено " << pipesToEdit.size() << " труб!" << endl;
        break;
    }
    case 0:
        cout << "Операция отменена." << endl;
        break;
    }
}

void PipelineSystem::saveToFile() {
    string filename = inputString("Введите имя файла для сохранения: ");
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка создания файла!" << endl;
        return;
    }

    file << pipes.size() << endl;
    for (const auto& pipe : pipes) {
        file << pipe.getId() << endl;
        file << pipe.getName() << endl;
        file << pipe.getLength() << endl;
        file << pipe.getDiameter() << endl;
        file << pipe.isInRepair() << endl;
    }

    file << stations.size() << endl;
    for (const auto& cs : stations) {
        file << cs.getId() << endl;
        file << cs.getName() << endl;
        file << cs.getTotalWorkshops() << endl;
        file << cs.getWorkingWorkshops() << endl;
        file << cs.getEfficiencyClass() << endl;
    }

    file.close();
    logger.log("Сохранение данных в файл: " + filename);
    cout << "Данные сохранены в файл: " << filename << endl;
}

void PipelineSystem::loadFromFile() {
    string filename = inputString("Введите имя файла для загрузки: ");
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    vector<Pipe> backupPipes = pipes;
    vector<CS> backupStations = stations;
    int backupNextPipeId = nextPipeId;
    int backupNextCSId = nextCSId;

    try {
        pipes.clear();
        stations.clear();

        int pipeCount;
        file >> pipeCount;
        file.ignore();

        for (int i = 0; i < pipeCount; i++) {
            Pipe pipe(0);
            int id;
            string name;
            double length;
            int diameter;
            bool inRepair;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> length;
            file >> diameter;
            file >> inRepair;
            file.ignore();

            pipe = Pipe(id);
            pipe.setName(name);
            pipe.setLength(length);
            pipe.setDiameter(diameter);
            pipe.setRepairStatus(inRepair);

            pipes.push_back(pipe);
            if (id >= nextPipeId) {
                nextPipeId = id + 1;
            }
        }

        int csCount;
        file >> csCount;
        file.ignore();

        for (int i = 0; i < csCount; i++) {
            CS cs(0);
            int id;
            string name;
            int totalWorkshops;
            int workingWorkshops;
            string efficiencyClass;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> totalWorkshops;
            file >> workingWorkshops;
            file.ignore();
            getline(file, efficiencyClass);

            cs = CS(id);
            cs.setName(name);
            cs.setTotalWorkshops(totalWorkshops);
            cs.setWorkingWorkshops(workingWorkshops);
            cs.setEfficiencyClass(efficiencyClass);

            stations.push_back(cs);
            if (id >= nextCSId) {
                nextCSId = id + 1;
            }
        }

        file.close();
        logger.log("Загрузка данных из файла: " + filename);
        cout << "Данные загружены из файла: " << filename << endl;

    }
    catch (...) {
        pipes = backupPipes;
        stations = backupStations;
        nextPipeId = backupNextPipeId;
        nextCSId = backupNextCSId;
        cout << "Ошибка загрузки файла! Данные восстановлены." << endl;
    }
}