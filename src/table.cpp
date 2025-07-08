//
// Created by Derrick Modad on 5/19/25.
//

#include "table.hpp"
#include <fstream>

Table::Table(std::string name) {
    tableName = name;
    tableFile = name + ".db";
}

void Table::insertRecord(Record record) {
    records.push_back(record);
}

std::vector<Record> Table::getRecords() {
    return records;
}

void printRecord(Record& record) {
    std::cout << record.id << ' ' << record.firstName << ' ' << record.lastName << std::endl;
}

void Table::saveToFile() {
    std::ofstream outFile(tableFile, std::ios::binary | std::ios::app);
    for (auto& record : records) {
        serialize(outFile, record);
    }
    outFile.close();
}

void Table::loadFromFile() {
    records.clear();
    std::ifstream inFile(tableFile, std::ios::binary | std::ios::in);
    Record record;
    while (deserialize(inFile, record)) {
        records.push_back(record);
    }
    inFile.close();
}

std::vector<Record> Table::selectWhere(std::function<bool(const Record &)> comparator) {
    std::vector<Record> results;
    for (auto& record : records) {
        if (comparator(record)) {
            results.push_back(record);
        }
    }
    return results;
}

void Table::deleteWhere(std::function<bool(const Record &)> comparator) {
    records.erase(std::remove_if(records.begin(), records.end(), comparator), records.end());
    //opting here for manual call to saveToFile to prevent unwanted changes
}

void Table::updateWhere(std::function<bool(const Record &)> comparator, std::function<void(const Record &)> updater) {

}


