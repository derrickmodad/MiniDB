//
// Created by Derrick Modad on 5/19/25.
//

#include "database.hpp"
#include "record.hpp"
#include "table.hpp"
#include <fstream>

Database::Database() {
    //make file for tables (if not created)
    tables.clear();
    std::ifstream inDBTableFile("dbTableFile.txt",std::ios::in);
    std::string tableName;
    while (getline(inDBTableFile, tableName)) {
        tables[tableName] = Table(tableName);
        //opting for manual load rn, but could do this: tables[tableName].loadFromFile();
    }
}
