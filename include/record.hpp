//
// Created by Derrick Modad on 5/26/25.
//

#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <vector>

class Record {
    std::vector<std::string> data;
public:
    Record() = default;
    Record(std::vector<std::string>&);
    bool serialize(std::ofstream& out) const;
    bool deserialize(std::ifstream& in);
    void printRecord() const;
    std::vector<std::string> getData() const;
    bool alterData(int, const std::string&);
};

#endif //RECORD_HPP
