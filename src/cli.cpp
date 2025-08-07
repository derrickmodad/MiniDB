//
// Created by Derrick Modad on 7/22/25.
//

#include "cli.hpp"

using CommandHandler = std::function<std::string(const std::vector<std::string>&)>;

std::vector<std::string> CLI::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

bool CLI::requiredArgumentsPresent(int args, int expected) {
    std::cout << std::flush;
    if (args == expected)
        return true;
    if (args > expected)
        std::cout << "Error: too many arguments" << std::endl;
    else if (args < expected)
        std::cout << "Error: too few arguments" << std::endl;
    return false;
}

void CLI::runCLI() {
    Table employees("Employees");
    std::string command;
    std::vector<std::string> args;
    do {
        std::cout << "> " << std::flush;
        std::getline(std::cin, command);
        args = split(command, ' ');
        if (args[0] == "insert") {
            if (requiredArgumentsPresent(args.size() - 1, 3)) {
                Record r;
                employees.insertRecord(
                    createRecord(std::stoi(args[1]),
                    std::strncpy(r.firstName, args[2].c_str(), sizeof(r.firstName)),
                    std::strncpy(r.lastName, args[3].c_str(), sizeof(r.lastName))));
                employees.saveToFile(); //this will cause duplicates to be saved if the file is loaded in memory
            }
        } else if (args[0] == "update") {
            //should receive "update {id} {col} {updated}"
            if (requiredArgumentsPresent(args.size() - 1, 3)) {
                employees.loadFromFile();
                auto comparator = [=](const Record& r1) -> bool {
                    return r1.id == std::stoi(args[1]);
                };
                auto updater = [=](Record& r1) -> void {
                    //ok, so this updates BUT it DUPLICATES rather than truly updating
                    if (args[2][0] == 'f') {
                        std::strncpy(r1.firstName, args[3].c_str(), sizeof(r1.firstName));
                    } else {
                        std::strncpy(r1.lastName, args[3].c_str(), sizeof(r1.lastName));
                    }
                };
                employees.updateWhere(comparator, updater);
                employees.saveToFile();
            }
        } else if (args[0] == "delete") {

        } else if (args[0] == "exit") {
            cont = false;
        } else if (args[0] == "select") {
            employees.loadFromFile();
            if (args[1] == "*") {
                auto rec = employees.getRecords();
                for (auto& r: rec) {
                    employees.printRecord(r);
                }
            }
        } else if (args[0] == "help") {

        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    } while (CLIActive);
}

void CLI::setup() {
    registerCommand("use", [this](const std::vector<std::string>& args) {
        return useHandler(args);
    });
    registerCommand("create", [this](const std::vector<std::string>& args) {
        return createHandler(args);
    });
    registerCommand("insert", [this](const std::vector<std::string>& args) {
        return insertHandler(args);
    });
    registerCommand("update", [this](const std::vector<std::string>& args) {
        return updateHandler(args);
    });
    registerCommand("delete", [this](const std::vector<std::string>& args) {
        return deleteHandler(args);
    });
    registerCommand("select", [this](const std::vector<std::string>& args) {
        return selectHandler(args);
    });
}

std::string CLI::useHandler(const std::vector<std::string>& args) {
    Table* lastUsed = currentTable; //this should save a pointer to the table before switching so it can be saved if switch is successful
    if (args.size() != 2) {
        return "syntax error - expected: use <table name>";
    }
    currentTable = db.lookupTable(args[1]);
    if (currentTable == nullptr) {
        currentTable = lastUsed;
        return "error: table not found";
    }
    lastUsed->saveToFile();
    currentTable->loadFromFile();
    return "active table: " + currentTable->getTableName();
}

std::string CLI::createHandler(const std::vector<std::string>& args) {
    //only for static table for current use - will expand on later with dynamic implementation
    if (args.size() != 2) {
        return "syntax error - expected: create <table name>";
    }
    Table* query = db.lookupTable(args[1]);
    if (query != nullptr) {
        return "error: table already exists";
    }
    db.appendTable(args[1]);
    return "successfully created " + args[1];
}

std::string CLI::insertHandler(const std::vector<std::string>& args) {
    return "not yet built";
}

std::string CLI::updateHandler(const std::vector<std::string>& args) {
    return "not yet built";
}

std::string CLI::deleteHandler(const std::vector<std::string>& args) {
    return "not yet built";
}

std::string CLI::selectHandler(const std::vector<std::string>& args) {
    return "not yet built";
}

std::string CLI::exitHandler(const std::vector<std::string>& args) {
    CLIActive = false;
    //use args to determine if to save changes
    return "success";
}

std::string CLI::helpHandler(const std::vector<std::string>& args) {
    //use args to determine what to help with, else print list with proper format
    return "Invalid syntax: help <command> (not yet built)";
}

CLI::CLI(Database& db) : db(db) {
    //constructor
    //build command table
    currentTable = nullptr;
    setup();
    CLIActive = true;
    runCLI();
}

void CLI::registerCommand(const std::string & name, const std::function<std::string(const std::vector<std::string> &)> & handler) {
    if (commands.find(name) != commands.end()) {
        std::cout << "Error: command already registered" << std::endl;
    } else {
        commands[name] = handler;
    }
}