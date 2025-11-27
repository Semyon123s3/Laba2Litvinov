#include "PipelineSystem.h"
#include <iostream>
#include <limits>
using namespace std;

PipelineSystem::PipelineSystem() : nextPipeId(1), nextCSId(1) {}

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
            cout << "Неверный выбор! Пожалуйста, выберите действие из меню." << endl;
        }

        cout << "\nНажмите Enter для продолжения...";
        cin.get();
    }
}