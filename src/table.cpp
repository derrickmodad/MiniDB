//
// Created by Derrick Modad on 5/19/25.
//

#include "table.hpp"
#include <fstream>
#include <iostream>

Table::Table(std::string& name, std::vector<Column>& columns) : tableName(name), columns(columns) {
    tableFile = "tables/" + name + ".db";
}

void Table::insertRecord(Record record) {
    records.push_back(record);
}

std::vector<Record> Table::getRecords() {
    return records;
}

void Table::saveToFile() {
    std::ofstream outFile(tableFile, std::ios::binary | std::ios::app);
    for (auto& record : records) {
        record.serialize(outFile);
    }
    outFile.close();
}

void Table::loadFromFile() {
    records.clear();
    std::ifstream inFile(tableFile, std::ios::binary | std::ios::in);

    if (!inFile.good()) {
        std::ofstream outFile(tableFile, std::ios::binary | std::ios::out);
        outFile.close();
        return; //return since file was created and nothing to load
    }

    Record record;
    while (record.deserialize(inFile)) {
        records.push_back(record);
    }
    inFile.close();
}

std::vector<Record> Table::selectWhere(std::function<bool(const Record &)> comparator) const {
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

void Table::updateWhere(std::function<bool(const Record &)> comparator, std::function<void(Record &)> updater) {
    for (auto& record : records) {
        if (comparator(record)) {
            updater(record);
        }
    }
}

std::string Table::getTableName() {
    return tableName;
}

std::string Table::getColumnNames() {
    std::string names;
    for (auto& col : columns) {
        names += "|" + col.name;
    }
    return names;
}


