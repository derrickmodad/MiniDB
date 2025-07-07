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

std::vector<Record> Table::getRecords() {
    return records;
}
