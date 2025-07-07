//
// Created by Derrick Modad on 5/29/25.
//

#include "record.hpp"
#include <fstream>

Record createRecord(unsigned int id, char firstName[], char lastName[]) {
    Record record;
    record.id = id;
    std::strncpy(record.firstName, firstName, sizeof(record.firstName) - 1);
    record.firstName[sizeof(record.firstName) - 1] = '\0';
    std::strncpy(record.lastName, lastName, sizeof(record.lastName) - 1);
    record.lastName[sizeof(record.lastName) - 1] = '\0';
    return record;
}

void serialize(std::ofstream &out, Record &record) {
    if (!out) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&record.id), sizeof(record.id));
    out.write(record.firstName, sizeof(record.firstName));
    out.write(record.lastName, sizeof(record.lastName));
}

bool deserialize(std::ifstream &in, Record &record) {
    if (!in) {
        std::cerr << "Error opening file" << std::endl;
    } else {
        in.read(reinterpret_cast<char *>(&record.id), sizeof(record.id));
        in.read(record.firstName, sizeof(record.firstName));
        in.read(record.lastName, sizeof(record.lastName));
    }

    return in.good();
}
