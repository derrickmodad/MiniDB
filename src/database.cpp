//
// Created by Derrick Modad on 5/19/25.
//

#include "database.hpp"
#include "cli.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

bool Database::checkTableFileExists() {
    std::ifstream inFile("dbTableFile.txt");
    if (!inFile.good()) {
        std::ofstream outFile("dbTableFile.txt");
        if (!outFile) {
            return false;
        }
        outFile.close();
    }
    inFile.close();
    return true;
}

bool Database::checkTableDirectoryExists() {
    std::filesystem::path dir = "tables";
    if (!std::filesystem::exists(dir)) {
        if (!std::filesystem::create_directory(dir)) {
            return false;
        }
    }
    return true;
}

Database::Database() {
    //make file for tables
    if (!checkTableFileExists()) {
        throw std::runtime_error("fatal error: Database file not found");
    }
    //make directory for tables
    if (!checkTableDirectoryExists()) {
        throw std::runtime_error("fatal error: Database directory not found");
    }
    tables.clear();
    std::ifstream inDBTableFile("dbTableFile.txt",std::ios::in);
    std::string tableInfo;
    while (getline(inDBTableFile, tableInfo)) {
        appendTable(tableInfo);
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
Table *Database::lookupTable(const std::string& query) {
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
        outDBTableFile << it->first << it->second.getColumnNames() << std::endl;
    }
    outDBTableFile.close();
    return true;
}

void Database::appendTable(std::string& tableInfo) {
    std::vector<std::string> splitTabInfo = CLI::split(tableInfo, '|');
    std::vector<Column> columns;
    for (int i = 1; i < splitTabInfo.size(); i++) {
        columns.emplace_back(Column{splitTabInfo[i]});
    }
    tables.emplace(splitTabInfo[0], Table(splitTabInfo[0], columns));
}



