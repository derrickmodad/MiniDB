//
// Created by Derrick Modad on 5/26/25.
//

#ifndef RECORD_HPP
#define RECORD_HPP

#include <iostream>

struct Record {
    unsigned int id;
    char firstName[32];
    char lastName[32];
};

Record createRecord(unsigned int id, char firstName[], char lastName[]);

void serialize(std::ofstream& out, Record& record);

bool deserialize(std::ifstream& in, Record &record);

#endif //RECORD_HPP
