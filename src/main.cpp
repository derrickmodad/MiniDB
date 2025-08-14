#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "database.hpp"
#include "cli.hpp"

int main() {
    Database engine;
    CLI terminal(engine);
    std::cout << "shutting down..." << std::endl;
}
