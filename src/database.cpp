//
// Created by Derrick Modad on 5/19/25.
//

#include "database.hpp"
#include "record.hpp"
#include "table.hpp"
#include <fstream>

bool Database::checkTableFileExists() {
    std::ifstream inFile("dbTableFile.txt");
    if (!inFile.good()) {
        std::ofstream outFile("dbTableFile.txt");
        if (!outFile) {
            return false;
        }
    }
    return true;
}

Database::Database() {
    //make file for tables (if not created)
    if (!checkTableFileExists()) {
        throw std::runtime_error("Database file not found");
    }
    tables.clear();
    std::ifstream inDBTableFile("dbTableFile.txt",std::ios::in);
    std::string tableName;
    while (getline(inDBTableFile, tableName)) {
        appendTable(tableName);
        //opting for manual load rn, but could do this: tables[tableName].loadFromFile();
    }
    inDBTableFile.close();
}

//final save before stopping database engine
Database::~Database() {
    saveTableFile();
    std::cout << "stopping database..." << std::endl;
}

//checks for table in map, returns if found, else returns nullptr
Table *Database::lookupTable(std::string query) {
    auto it =  tables.find(query);
    if (it != tables.end()) {
        return &(it->second);
    }
    return nullptr;
}

//returns true on successful save, false otherwise
bool Database::saveTableFile() {
    std::ofstream outDBTableFile("dbTableFile.txt", std::ios::out);
    if (!outDBTableFile.good())
        return false;
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        outDBTableFile << it->first << std::endl;
    }
    outDBTableFile.close();
    return true;
}

void Database::appendTable(std::string newTable) {
    tables.emplace(newTable, Table(newTable));
}



