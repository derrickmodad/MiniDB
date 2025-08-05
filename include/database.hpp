//
// Created by Derrick Modad on 5/19/25.
//

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "record.hpp"
#include "table.hpp"
#include <unordered_map>

class Database {
private:
    std::unordered_map<std::string, Table> tables;

public:
    Database();
    //how to share the tables in the map? i don't know about sharing a pointer
};

#endif //DATABASE_HPP
