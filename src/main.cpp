#include <iostream>
#include <vector>
#include "database.hpp"
#include "table.hpp"
#include "record.hpp"

int main() {
    Table employees("Employees");
    employees.insertRecord(createRecord(0, "Derrick", "Modad"));
    employees.insertRecord(createRecord(1, "John", "Smith"));
    employees.insertRecord(createRecord(2, "Mary", "Jane"));
    employees.saveToFile();

    auto matches = employees.selectWhere([](const Record& record) { return record.id == 0;});
    for (auto& match : matches) {
        std::cout << match.firstName << std::endl;
    }

    employees.deleteWhere([](const Record& record) { return record.id == 1;});
    employees.saveToFile();

    employees.updateWhere([](const Record& record) {return record.id == 2;},
        [](Record& record) {
            std::strncpy(record.lastName, "Doe", sizeof(record.lastName));
            std::strncpy(record.firstName, "Jane", sizeof(record.firstName));
        });

    auto records = employees.getRecords();
    for (auto& record: records) {
        employees.printRecord(record);
    }
}
