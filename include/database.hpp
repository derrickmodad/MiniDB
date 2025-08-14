//
// Created by Derrick Modad on 5/19/25.
//

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "table.hpp"
#include <unordered_map>
#include <string>

class Database {
private:
    std::unordered_map<std::string, Table> tables;
    bool checkTableFileExists();
    bool checkTableDirectoryExists();

public:
    Database();
    ~Database();
    Table* lookupTable(const std::string&);
    bool saveTableFile();
    void appendTable(std::string&);
};

#endif //DATABASE_HPP
