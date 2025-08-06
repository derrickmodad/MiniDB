//
// Created by Derrick Modad on 7/22/25.
//

#ifndef CLI_HPP
#define CLI_HPP

#include "table.hpp"
#include "database.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

class CLI {
private:
    Database& db;
    Table* currentTable;
    bool CLIActive;
    //key - string, value - command handler (with return type string)
    //  note: the command handler will call the actual command function built in table.cpp and unpack its return value (if applicable)
    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> commands;
    std::vector<std::string> split (const std::string &, char);
    bool requiredArgumentsPresent(int, int);
    void runCLI();//initialize CLI, called from constructor
    void setup(); //default grouping of command registering, called from constructor

    //handlers
    std::string useHandler(const std::vector<std::string> &);
    std::string createHandler(const std::vector<std::string> &);
    std::string insertHandler(const std::vector<std::string>&);
    std::string updateHandler(const std::vector<std::string>&);
    std::string deleteHandler(const std::vector<std::string>&);
    std::string selectHandler(const std::vector<std::string>&);
    std::string exitHandler(const std::vector<std::string>&);
    std::string helpHandler(const std::vector<std::string>&);
public:
    CLI(Database&);
    void registerCommand(const std::string&, const std::function<std::string(const std::vector<std::string>&)>&); //individual registering of commands, left public for extensibility
};



#endif //CLI_HPP
