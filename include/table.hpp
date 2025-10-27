//
// Created by Derrick Modad on 5/19/25.
//

#ifndef TABLE_HPP
#define TABLE_HPP

#include "record.hpp"
#include <functional>
#include <vector>
#include <string>

struct Column {
    std::string name;
    std::string type = "string"; //string type for all (temporarily)
};

class Table {
private:
    std::string tableName;
    std::string tableFile;
    std::vector<Column> columns;
    std::vector<Record> records;

public:
    Table(std::string&, std::vector<Column>&);

    //record insertion and viewing
    void insertRecord(Record record);
    std::vector<Record> getRecords();

    //save and load
    void saveToFile();
    void loadFromFile();

    //select, delete, update
    std::vector<Record> selectWhere(std::function<bool(const Record&)> comparator) const;
    int deleteWhere(std::function<bool(const Record&)> comparator);
    void updateWhere(std::function<bool(const Record&)> comparator, std::function<void(Record&)> updater);

    std::string getTableName();
    std::string getColumnNames();
    std::vector<Column> getColumns();
    bool columnExists(std::string columnName);
    int getColumnCount();
};

#endif //TABLE_HPP
