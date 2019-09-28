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

class memory {
private:
    int program[2000];

public:
    memory(std::string input);
    void print();
    int read(int address);
    void write(int address, int data);

};


#endif //PROJECT1OSCPP_MEMORY_H
