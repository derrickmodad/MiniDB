//
// Created by Derrick Modad on 5/19/25.
//

#ifndef TABLE_HPP
#define TABLE_HPP

#include <functional>

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

    //record insertion and viewing
    void insertRecord(Record record);
    std::vector<Record> getRecords();
    void printRecord(Record& record);

    //save and load
    void saveToFile();
    void loadFromFile();

    //select, delete, update
    std::vector<Record> selectWhere(std::function<bool(const Record&)> comparator) const;
    void deleteWhere(std::function<bool(const Record&)> comparator);
    void updateWhere(std::function<bool(const Record&)> comparator, std::function<void(Record&)> updater);

    std::string getTableName();
};

#endif //TABLE_HPP
