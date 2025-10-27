//
// Created by Derrick Modad on 7/22/25.
//

#include "cli.hpp"
#include <sstream>
#include <cctype>
#include <iostream>
#include <unordered_set>

std::vector<std::string> CLI::split(const std::string& str, const char& delimiter) {
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
        std::string currentTableName = (activeCurrentTable() ? currentTable->getTableName() : "");
        std::cout << currentTableName << "> " << std::flush;
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
    std::string tableInfo;
    std::unordered_set<std::string> columns;
    for (int i = 1; i < args.size(); i++) {
        //check for illegal characters
        if (contains(args[i], '=')) {
            std::cout << "error: " << args[i] << " contains '=' (illegal character)";
            return "";
        }

        //check for duplicate column names
        if (columns.contains(args[i])) {
            std::cout << "error: cannot have duplicate column names (" << args[i] << ")";
            return "";
        }
        columns.insert(args[i]);
        tableInfo += args[i];
        if (i != args.size() - 1) {
            tableInfo += "|";
        }
    }
    //add table to table map
    db.appendTable(tableInfo);
    return "successfully created " + args[1];
}

//TODO: add an easy mode for inserting
//  it will show the columns of the table like:
//  insert -e (<- enter) (-e is flag for easy mode)
//  displays each column like:
//  id:
//  name:
//  email:
std::string CLI::insertHandler(const std::vector<std::string>& args) {
    if (!activeCurrentTable())
        return "error: no table selected";

    //get count of columns in currentTable and check for args to match
    if (args.size() - 1 != currentTable->getColumnCount())
        return "error: supplied arguments do not match number of columns in table";

    std::vector<std::string> trimmedArgs(args.begin() + 1, args.end());
    for (const std::string& arg : trimmedArgs) {
        if (contains(arg, '=')) {
            std::cout << "error: " << arg << " contains '=' (illegal character)";
            return "";
        }
    }
    Record newRecord(trimmedArgs);
    currentTable->insertRecord(newRecord);
    return "successfully inserted";
}

//update column="value" with column="value" column="value"
std::string CLI::updateHandler(const std::vector<std::string>& args) {
    if (!activeCurrentTable())
        return "error: no table selected";

    if (args.size() - 1 < 1)
        return "error: at least one argument must be supplied";

    //find with (this is necessary for update statements)
    int preWithArgsCount = -1;                                                           //args to be passed to comparator
    bool withPresent = false;
    for (std::string arg : args) {
        toLowerCase(arg);
        if (arg != "with")
            preWithArgsCount++;
        else if (arg == "with") {
            withPresent = true;
            if (preWithArgsCount < 1)
                return "error: identifier columns must be supplied before \"with\"";
            break;
        }
    }

    if (!withPresent)
        return R"(syntax error - missing "with" - expected: update <column>="<value>" with <column>="<value>")";

    if (preWithArgsCount == args.size() - 2)
        return R"(error: updater columns must be supplied after "with")";

    std::vector<std::string> preColumn, postColumn, preValue, postValue;
    const std::vector<std::string> preWith(args.begin(), args.begin() + preWithArgsCount + 1),
                                   postWith(args.begin() + preWithArgsCount + 1, args.end());
    if (!parseColVal(preColumn, preValue, preWith))
        return "";
    if (!parseColVal(postColumn, postValue, postWith))
        return "";

    auto comparator = [&](const Record& record) {
        const std::vector<std::string> store = record.getData();
        const std::vector<Column>& cols = currentTable->getColumns();

        for (int i = 0; i < preColumn.size(); i++) {
            const std::string& colName = preColumn[i];
            const std::string& val = preValue[i];
            int colIndex = -1;
            for (int j = 0; j < cols.size(); j++) {
                if (cols[j].name == colName) {
                    colIndex = j;
                    break;
                }
            }

            if (colIndex == -1 || store[colIndex] != val)
                return false;
        }
        return true;
    };

    auto updater = [&](Record& record) {
        const std::vector<Column>& cols = currentTable->getColumns();
        std::vector<int> colIndices;
        for (const std::string& colName : postColumn) {
            int colIndex = -1;
            for (int j = 0; j < cols.size(); j++) {
                if (cols[j].name == colName) {
                    colIndex = j;
                    colIndices.push_back(colIndex);
                    break;
                }
            }
            if (colIndex == -1) {
                std::cout << "error: column not found";
                return; //returning early to prevent adding column
            }
        }

        for (int i = 0; i < colIndices.size(); i++) {
            if (!record.alterData(colIndices[i], postValue[i]))
                std::cout << "error: failed to alter record" << std::endl;
        }
    };

    currentTable->updateWhere(comparator, updater);
    return "success";
}

//delete column="value"
std::string CLI::deleteHandler(const std::vector<std::string>& args) {
    if (!activeCurrentTable())
        return "error: no table selected";

    if (args.size() - 1 < 1)
        return "error: at least one argument must be supplied (to delete all, use: delete *)";

    if (args[1] == "*") {
        std::cout << "warning: delete all records of the current table? (delete all/cancel)\n> ";
        std::string confirm;
        std::getline(std::cin, confirm);
        toLowerCase(confirm);
        if (confirm != "delete all")
            return "delete: delete not confirmed";

        //delete all records
        currentTable->deleteWhere([&](const Record&){return true;});
        return "-- all records deleted";
    }

    //find certain record(s) to be deleted
    std::vector<std::string> column, value;
    if (!parseColVal(column, value, args))
        return "";

    auto comparator = [&](const Record& record) {
        const std::vector<std::string> store = record.getData();        //get the data
        const std::vector<Column>& cols = currentTable->getColumns();   //get the columns

        for (int i = 0; i < column.size(); i++) {                       //for each column in arguments
            const std::string& colName = column[i];                     //save name of argument column
            const std::string& val = value[i];                          //save search value
            int colIndex = -1;                                          //set found index to -1
            for (int j = 0; j < cols.size(); j++) {                     //loop through columns of table
                if (cols[j].name == colName) {                          //if the name matches, save index and exit loop
                    colIndex = j;
                    break;
                }
            }

            if (colIndex == -1 || store[colIndex] != val)               //if not found OR value is not right, return false
                return false;
        }
        return true;                                                    //value is right at correct index, return true
    };

    int deletedCount = currentTable->deleteWhere(comparator);
    std::cout << "Deleted " << deletedCount << " record" << (deletedCount == 1 ? "" : "s") << std::endl;
    return "success";
}

std::string CLI::selectHandler(const std::vector<std::string>& args) {
    if (!activeCurrentTable())
        return "error: no table selected";

    if (args.size() - 1 < 1)
        return "error: at least one argument must be supplied";

    //select *
    if (args[1] == "*") {
        for (auto const &record : currentTable->getRecords()) {
            record.printRecord();
        }
        return "-- all records printed";
    }

    //select column=value
    //tokenize search conditions and check for syntax errors
    //vectors to hold multiple arguments
    std::vector<std::string> column, value;

    if (!parseColVal(column, value, args))
        return "";

    auto comparator = [&](const Record& record) {
        const std::vector<std::string> store = record.getData();
        const std::vector<Column>& cols = currentTable->getColumns();

        for (int i = 0; i < column.size(); i++) {
            const std::string& colName = column[i];
            const std::string& val = value[i];
            int colIndex = -1;
            for (int j = 0; j < cols.size(); j++) {
                if (cols[j].name == colName) {
                    colIndex = j;
                    break;
                }
            }

            if (colIndex == -1 || store[colIndex] != val)
                return false;
        }
        return true;
    };

    std::vector<Record> results = currentTable->selectWhere(comparator);

    //print results (may be updated to support shipping results)
    for (Record const &r : results) {
        r.printRecord();
    }

    return "success";
}

std::string CLI::exitHandler(const std::vector<std::string>& args) {
    if (currentTable != nullptr)
        currentTable->saveToFile(); //save contents of current table OR could have a commit function
    CLIActive = false;
    //use args to determine if to save changes
    return "success";
}

std::string CLI::helpHandler(const std::vector<std::string>& args) {
    //use args to determine what to help with, else print list with proper format
    return "Invalid syntax: help <command> (not yet built)";
}

bool CLI::parseColVal(std::vector<std::string>& column, std::vector<std::string>& value, const std::vector<std::string>& args) const {
    for (int i = 1; i < args.size(); i++) {
        std::vector<std::string> parts = split(args[i], '=');
        if (parts.size() > 2 || parts.empty()) {
            std::cout << R"(error: illegal character '=' found)";
            return false;
        }

        if (parts[1].empty()) {
            std::cout << "error: no value specified for " + parts[0];
            return false;
        }

        if (!currentTable->columnExists(parts[0])) {
            std::cout << "error: column " + parts[0] + " not found";
            return false;
        }

        if (parts[1][0] != '"' || parts[1][parts[1].size() - 1] != '"') {
            std::cout << "error: value " + parts[1] + " is not a valid string value";
            return false;
        }

        column.push_back(parts[0]);
        value.push_back(parts[1].substr(1, parts[1].size() - 2)); //leave out quotation marks
    }
    return true;
}

bool CLI::contains(const std::string& str, const char& character) {
    for (const char c : str) {
        if (c == character)
            return true;
    }
    return false;
}

void CLI::toLowerCase(std::string& str) {
    for (char& c : str)
        c = std::tolower(static_cast<unsigned char>(c));
}


bool CLI::activeCurrentTable() {
    return currentTable != nullptr;
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