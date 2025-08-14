//
// Created by Derrick Modad on 7/22/25.
//

#include "cli.hpp"
#include <complex>

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
    std::string command;
    std::vector<std::string> args;
    do {
        std::cout << "> " << std::flush;
        std::getline(std::cin, command);
        args = split(command, ' ');
        auto it = commands.find(args[0]);
        if (it != commands.end()) {
            std::cout << it->second(args) << std::endl;
        } else {
            std::cout << "error: unknown command" << std::endl;
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
    registerCommand("exit", [this](const std::vector<std::string>& args) {
        return exitHandler(args);
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
    if (lastUsed != nullptr)
        lastUsed->saveToFile();
    currentTable->loadFromFile();
    return "active table: " + currentTable->getTableName();
}

std::string CLI::createHandler(const std::vector<std::string>& args) {
    //check that at least one column is passed along with name
    if (args.size() <= 2) {
        return "syntax error - expected: create <table name> <column name> (at least 1 column name must be present)";
    }

    //check if table exists
    Table* query = db.lookupTable(args[1]);
    if (query != nullptr) {
        return "error: table already exists";
    }

    //build args into string for table
    std::string tableInfo = "";
    for (int i = 1; i < args.size(); i++) {
        tableInfo += args[i];
        if (i != args.size() - 1) {
            tableInfo += "|";
        }
    }

    //add table to table map
    db.appendTable(tableInfo);
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

CLI::CLI(Database& database) : db(database) {
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