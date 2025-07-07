//
// Created by Derrick Modad on 5/19/25.
//

#ifndef TABLE_HPP
#define TABLE_HPP

#include "record.hpp"
#include <vector>
#include <string>

class Table {
private:
    std::string tableName;
    std::string tableFile;
    std::vector<Record> records;
public:
    Table(std::string name);
    void insertRecord(Record record);
    void saveToFile();
    void loadFromFile();
    std::vector<Record> getRecords();
};

#endif //TABLE_HPP
