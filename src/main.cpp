#include <iostream>
#include <vector>
#include "database.hpp"
#include "table.hpp"
#include "record.hpp"

std::vector<Record> generateRecords();

int main() {
    auto receive = generateRecords();
    for (auto & i : receive) {
        // std::cout << i.id << ' ' << i.firstName << ' ' << i.lastName << std::endl;
        writeRecord(i);
    }

    auto records = readRecords();
    for (auto & i : records) {
        std::cout << i.id << ' ' << i.firstName << ' ' << i.lastName << std::endl;
    }
}

std::vector<Record> generateRecords() {
    std::vector<Record> records;

    //generate vector of records for testing
    Record record = createRecord(0, "Derrick", "Modad");
    records.push_back(record);
    record = createRecord(1, "Test", "Test");
    records.push_back(record);

    return records;
}
