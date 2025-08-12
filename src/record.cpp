//
// Created by Derrick Modad on 5/29/25.
//

#include "record.hpp"
#include <fstream>
#include <iostream>


Record::Record(std::vector<std::string>& vals) : data(vals) {}

bool Record::serialize(std::ofstream &out) const {
    if (!out) {
        std::cout << "Error opening file" << std::endl;
        return false;
    }
    size_t recSize = data.size();
    out.write(reinterpret_cast<const char *>(&recSize), sizeof(recSize));

    for (const auto& item : data) {
        size_t len = item.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(item.c_str(), len);
    }
    return true;
}

bool Record::deserialize(std::ifstream &in) {
    if (!in.good())
        return false; // may opt to display error here
    data.clear();
    size_t recSize;
    in.read(reinterpret_cast<char *>(&recSize), sizeof(recSize));

    for (size_t i = 0; i < recSize; i++) {
        size_t len;
        in.read(reinterpret_cast<char *>(&len), sizeof(len));
        std::string item(len, '\0');
        in.read(&item[0], len);
        data.push_back(std::move(item));
    }
    return true;
}

void Record::printRecord() const {
    for (const auto& item : data) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}
