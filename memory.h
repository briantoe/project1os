//
// Created by osboxes on 9/16/19.
//

#ifndef PROJECT1OSCPP_MEMORY_H
#define PROJECT1OSCPP_MEMORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <climits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
static inline std::string &ltrim(std::string &s);
static inline std::string &rtrim(std::string &s);
static inline std::string &trim(std::string &s);

class memory {
private:
    int program[2000];

public:
    memory(std::string input);
    void print();
    int readMem(int address);
    void writeMem(int address, int data);

};


#endif //PROJECT1OSCPP_MEMORY_H
