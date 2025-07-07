#include <iostream>
#include <vector>
#include "database.hpp"
#include "table.hpp"
#include "record.hpp"

void printTable(Table& table) {
    table.loadFromFile();
    auto records = table.getRecords();
    for (auto record : records) {
        std::cout << record.id << record.firstName << record.lastName << std::endl;
    }
}

int main() {
    Table employees("Employees");
    employees.insertRecord(createRecord(0, "Derrick", "Modad"));
    employees.saveToFile();

    Table employees2("Employees2");
    employees2.insertRecord(createRecord(1, "Derrick", "Modad"));
    employees2.saveToFile();

    printTable(employees);
    printTable(employees2);
}
