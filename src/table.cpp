//
// Created by Derrick Modad on 5/19/25.
//

#include "table.hpp"
#include <fstream>

void writeRecord(Record &record) {
    std::ofstream outFile;
    outFile.open("../record.db", std::ios::binary | std::ios::app);
    serialize(outFile, record);
    outFile.close();
}

std::vector<Record> readRecords() {
    std::ifstream inFile;
    inFile.open("../record.db", std::ios::binary | std::ios::in);
    Record record;
    std::vector<Record> records;
    while (deserialize(inFile, record)) {
        records.push_back(record);
    }
    inFile.close();
    return records;
}
