#include "PipelineSystem.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include "GasNetwork.h"
#include "Constants.h" 
using namespace std;

PipelineSystem::PipelineSystem() : nextPipeId(1), nextCSId(1) {
    gasNetwork = new GasNetwork(pipes, stations);   
}

PipelineSystem::~PipelineSystem() {
    delete gasNetwork;   
}

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
    auto it = pipes.find(id);          
    if (it != pipes.end()) {
        return &(it->second);           
    }
    return nullptr;
}

CS* PipelineSystem::findCSById(int id) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        return &(it->second);
    }
    return nullptr;
}

void PipelineSystem::showAllPipes() {
    cout << "\n--- ВСЕ ТРУБЫ ---" << endl;
    if (pipes.empty()) {
        cout << "Трубы не добавлены." << endl;
    }
    else {
        for (const auto& pair : pipes) {   
            pair.second.display();
        }
    }
}


void PipelineSystem::showAllCS() {
    cout << "\n--- ВСЕ КС ---" << endl;
    if (stations.empty()) {
        cout << "КС не добавлены." << endl;
    }
    else {
        for (const auto& pair : stations) {
            pair.second.display();
        }
    }
}

void PipelineSystem::addPipe() {
    cout << "\n=== ДОБАВЛЕНИЕ ТРУБЫ ===" << endl;

    Pipe newPipe(nextPipeId++);

    newPipe.setName(inputString("Введите километровую отметку (название): "));
    newPipe.setLength(inputDouble("Введите длину трубы (км): ", 0.1, 10000.0));

    cout << "\nДопустимые диаметры труб:\n";
    cout << "==========================\n";
    for (const auto& pair : PIPE_DIAMETERS_CAPACITY) {
        cout << "  " << pair.first << " мм -> " << pair.second << " млн м³/день\n";
    }
    cout << "==========================\n";

    int diameter;
    bool valid = false;
    while (!valid) {
        diameter = inputInt("Выберите диаметр из списка выше (мм): ", 1, 5000);
        if (PIPE_DIAMETERS_CAPACITY.find(diameter) != PIPE_DIAMETERS_CAPACITY.end()) {
            valid = true;
        }
        else {
            cout << "Ошибка! Диаметр " << diameter << " мм не поддерживается.\n";
            cout << "Допустимые диаметры: ";
            for (const auto& pair : PIPE_DIAMETERS_CAPACITY) {
                cout << pair.first << " ";
            }
            cout << "мм\n";
        }
    }

    newPipe.setDiameter(diameter);


    int repairStatus = inputInt("В ремонте (1-да, 0-нет): ", 0, 1);
    newPipe.setRepairStatus(repairStatus == 1);

    pipes[newPipe.getId()] = newPipe;
    logger.log("Добавлена труба ID: " + to_string(newPipe.getId()) +
        ", название: " + newPipe.getName() +
        ", диаметр: " + to_string(diameter) + " мм");

    cout << "\n✓ Труба успешно добавлена!" << endl;
}

void PipelineSystem::addCS() {
    cout << "\n=== ДОБАВЛЕНИЕ КОМПРЕССОРНОЙ СТАНЦИИ ===" << endl;

    CS newCS(nextCSId++);

    newCS.setName(inputString("Введите название КС: "));
    newCS.setTotalWorkshops(inputInt("Введите общее количество цехов: ", 1, 1000));
    newCS.setWorkingWorkshops(inputInt("Введите количество работающих цехов: ", 0, newCS.getTotalWorkshops()));
    newCS.setEfficiencyClass(inputString("Введите класс эффективности: "));

    stations[newCS.getId()] = newCS;
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
        for (const auto& pair : pipes) {
            pair.second.display();  
        }
    }

    cout << "\n--- КОМПРЕССОРНЫЕ СТАНЦИИ (" << stations.size() << ") ---" << endl;
    if (stations.empty()) {
        cout << "КС не добавлены." << endl;
    }
    else {
        for (const auto& pair : stations) {
            pair.second.display();  
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

    if (pipes.erase(id)) {
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

    if (stations.erase(id)) {
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
    unordered_map<int, Pipe*> foundPipes;   

    switch (choice) {
    case 1: {
        string name = inputString("Введите название для поиска: ");
        for (auto& pair : pipes) {   
            if (pair.second.getName().find(name) != string::npos) {
                foundPipes[pair.first] = &pair.second;   
            }
        }
        break;
    }
    case 2: {
        bool status = inputInt("В ремонте (1-да, 0-нет): ", 0, 1) == 1;
        for (auto& pair : pipes) {
            if (pair.second.isInRepair() == status) {
                foundPipes[pair.first] = &pair.second;
            }
        }
        break;
    }
    case 3:
        for (auto& pair : pipes) {
            foundPipes[pair.first] = &pair.second;
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
    unordered_map<int, CS*> foundCS;   

    switch (choice) {
    case 1: {
        string name = inputString("Введите название для поиска: ");
        for (auto& pair : stations) {
            if (pair.second.getName().find(name) != string::npos) {
                foundCS[pair.first] = &pair.second;
            }
        }
        break;
    }
    case 2: {
        double minPercent = inputDouble("Минимальный процент незадействованных цехов: ", 0, 100);
        double maxPercent = inputDouble("Максимальный процент незадействованных цехов: ", minPercent, 100);

        for (auto& pair : stations) {
            double percent = pair.second.getUnusedPercentage();
            if (percent >= minPercent && percent <= maxPercent) {
                foundCS[pair.first] = &pair.second;
            }
        }
        break;
    }
    case 3:
        for (auto& pair : stations) {
            foundCS[pair.first] = &pair.second;
        }
        break;
    }

    showCSList(foundCS);  
    logger.log("Поиск КС: найдено " + to_string(foundCS.size()) + " объектов");
}

void PipelineSystem::showPipesList(const unordered_map<int, Pipe*>& pipeMap) {
    cout << "\n--- НАЙДЕННЫЕ ТРУБЫ (" << pipeMap.size() << ") ---" << endl;
    if (pipeMap.empty()) {
        cout << "Трубы не найдены." << endl;
    }
    else {
        for (const auto& pair : pipeMap) {   
            pair.second->display();
        }
    }
}

void PipelineSystem::showCSList(const unordered_map<int, CS*>& csMap) {
    cout << "\n--- НАЙДЕННЫЕ КС (" << csMap.size() << ") ---" << endl;
    if (csMap.empty()) {
        cout << "КС не найдены." << endl;
    }
    else {
        for (const auto& pair : csMap) {
            pair.second->display();
        }
    }
}
void PipelineSystem::calculateMaxFlow() {
    gasNetwork->showMaxFlow();
}

void PipelineSystem::calculateShortestPath() {
    gasNetwork->showShortestPath();
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
    cout << "13. Соединить КС в сеть" << endl;
    cout << "14. Разорвать соединение" << endl;
    cout << "15. Показать сеть" << endl;
    cout << "16. Топологическая сортировка" << endl;
    cout << "17. Рассчитать максимальный поток" << endl;
    cout << "18. Найти кратчайший путь" << endl;

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

        case 13:
            connectObjects();
            break;
        case 14:
            disconnectObjects();
            break;
        case 15:
            showNetwork();
            break;
        case 16:
            showTopologicalSort();
            break;
        case 17:
            calculateMaxFlow();
            break;
        case 18:
            calculateShortestPath();
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
    cout << "\n=== ПАКЕТНОЕ РЕДАКТИРОВАНИЕ ТРУБ ===" << endl;

    if (pipes.empty()) {
        cout << "Нет доступных труб для редактирования." << endl;
        return;
    }

    unordered_map<int, Pipe*> foundPipes;
    cout << "Сначала выполним поиск труб:" << endl;
    searchPipes();   

    cout << "\nВведите ID труб через пробел для редактирования (или 'all' для всех найденных): ";
    string input;
    getline(cin, input);

    unordered_map<int, Pipe*> pipesToEdit;

    if (input == "all") {
        for (auto& pair : pipes) {
            pipesToEdit[pair.first] = &pair.second;
        }
    }
    else {
        stringstream ss(input);
        int id;
        while (ss >> id) {
            Pipe* pipe = findPipeById(id);
            if (pipe) {
                pipesToEdit[id] = pipe;   
            } else {
                cout << "Труба с ID " << id << " не найдена!" << endl;
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
        for (auto& pair : pipesToEdit) {  
            pair.second->setRepairStatus(newStatus);
        }
        logger.log("Пакетное изменение статуса труб: " + to_string(pipesToEdit.size()) + " объектов");
        cout << "Статус " << pipesToEdit.size() << " труб изменен!" << endl;
        break;
    }
    case 2: {
        for (auto& pair : pipesToEdit) {
            pipes.erase(pair.first);   
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

void PipelineSystem::connectObjects() {
    cout << "\n=== СОЕДИНЕНИЕ КОМПРЕССОРНЫХ СТАНЦИЙ ===" << endl;

    if (stations.size() < 2) {
        cout << "Для соединения нужно хотя бы 2 КС!" << endl;
        return;
    }

    showAllCS();

    int startId, endId, diameter;

    startId = inputInt("Введите ID КС входа: ", 1, nextCSId - 1);
    endId = inputInt("Введите ID КС выхода: ", 1, nextCSId - 1);

    if (startId == endId) {
        cout << "Ошибка: нельзя соединить КС саму с собой!" << endl;
        return;
    }

    cout << "\nДопустимые диаметры труб:\n";
    cout << "==========================\n";

    int index = 1;
    unordered_map<int, int> menuToDiameter; 

    for (const auto& pair : PIPE_DIAMETERS_CAPACITY) {
        cout << index << ". " << pair.first << " мм (пропускная способность: "
            << pair.second << " млн м³/день)\n";
        menuToDiameter[index] = pair.first;
        index++;
    }
    cout << "==========================\n";

    int choice = inputInt("Выберите диаметр трубы (1-" +
        to_string(PIPE_DIAMETERS_CAPACITY.size()) + "): ",
        1, PIPE_DIAMETERS_CAPACITY.size());

    diameter = menuToDiameter[choice];

    if (gasNetwork->connectOrCreatePipe(startId, endId, diameter, pipes, nextPipeId)) {
        cout << "Соединение успешно создано!" << endl;
        logger.log("Создано соединение: КС" + to_string(startId) + " -> КС" + to_string(endId) +
            " через трубу диаметром " + to_string(diameter) + " мм");
    }
}

void PipelineSystem::disconnectObjects() {
    cout << "\n=== РАЗРЫВ СОЕДИНЕНИЯ ===" << endl;

    if (pipes.empty()) {
        cout << "Нет труб." << endl;
        return;
    }

    showAllPipes();
    int pipeId = inputInt("Введите ID трубы для разрыва соединения: ", 1, nextPipeId - 1);

    if (gasNetwork->disconnectPipe(pipeId)) {
        cout << "Соединение разорвано." << endl;
        logger.log("Разорвано соединение для трубы ID: " + to_string(pipeId));
    }
}

void PipelineSystem::showNetwork() {
    gasNetwork->showNetwork();
}

void PipelineSystem::showTopologicalSort() {
    vector<int> sorted = gasNetwork->topologicalSort();

    if (sorted.empty()) {
        cout << "Топологическая сортировка невозможна." << endl;
    }
    else {
        cout << "\n=== ТОПОЛОГИЧЕСКАЯ СОРТИРОВКА ===" << endl;
        cout << "Порядок обработки КС: ";
        for (size_t i = 0; i < sorted.size(); ++i) {
            cout << "КС" << sorted[i];
            if (i < sorted.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        logger.log("Выполнена топологическая сортировка.");
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
    for (const auto& pair : pipes) {
        const Pipe& pipe = pair.second;
        file << pipe.getId() << endl;
        file << pipe.getName() << endl;
        file << pipe.getLength() << endl;
        file << pipe.getDiameter() << endl;
        file << pipe.isInRepair() << endl;

        file << pipe.getIsConnected() << endl;
        if (pipe.getIsConnected()) {
            file << pipe.getStartStationId() << endl;
            file << pipe.getEndStationId() << endl;
        }
    }

    file << stations.size() << endl;
    for (const auto& pair : stations) {
        const CS& cs = pair.second;
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

    unordered_map<int, Pipe> backupPipes = pipes;
    unordered_map<int, CS> backupStations = stations;
    int backupNextPipeId = nextPipeId;
    int backupNextCSId = nextCSId;

    try {
        pipes.clear();
        stations.clear();

        int pipeCount;
        file >> pipeCount;
        file.ignore();

        for (int i = 0; i < pipeCount; i++) {
            int id;
            string name;
            double length;
            int diameter;
            bool inRepair;
            bool isConnected; 
            int startId = -1, endId = -1;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> length >> diameter >> inRepair;

            file >> isConnected;
            if (isConnected) {
                file >> startId >> endId;
            }
            file.ignore(); 

            Pipe pipe(id);
            pipe.setName(name);
            pipe.setLength(length);
            pipe.setDiameter(diameter);
            pipe.setRepairStatus(inRepair);

            if (isConnected) {
                pipe.connect(startId, endId);
            }

            pipes[id] = pipe;
            if (id >= nextPipeId) {
                nextPipeId = id + 1;
            }
        }

        int csCount;
        file >> csCount;
        file.ignore();

        for (int i = 0; i < csCount; i++) {
            int id;
            string name;
            int totalWorkshops;
            int workingWorkshops;
            string efficiencyClass;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> totalWorkshops >> workingWorkshops;
            file.ignore();
            getline(file, efficiencyClass);

            CS cs(id);
            cs.setName(name);
            cs.setTotalWorkshops(totalWorkshops);
            cs.setWorkingWorkshops(workingWorkshops);
            cs.setEfficiencyClass(efficiencyClass);

            stations[id] = cs;
            if (id >= nextCSId) {
                nextCSId = id + 1;
            }
        }

        file.close();

        delete gasNetwork;
        gasNetwork = new GasNetwork(pipes, stations);

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