//
// Created by osboxes on 9/16/19.
//

#include "memory.h"

memory::memory(std::string input) {
    std::ifstream infile;
    infile.open(input);
    int instruction;
    int index = 0;
    std::string line;
    while (!infile.eof()) {
        int instruction;
        std::getline(infile, line);
        trim(line);
        std::stringstream ss(line);
        if(line == "\n")
            continue;
        if(line[0] == '/'){
            continue;
        }
        if(line == "\0")
        {
            continue;
        }
        if (line[0] == '.') {
            std::string num = line.substr(1, line.size() - 1);
            int temp = std::stoi(num);
            index = temp;
            continue;
        }
        else
        {
//            std::cout << "Reading in instruction: " << instruction << std::endl;
            ss >> instruction;
        }

        program[index] = instruction;
        index++;
    }

}

void memory::print() {

    for(int i = 0; i < 2000; i ++)
    {
        std::cout << program[i] << std::endl;
    }
//    std:: cout << program[0] << std::endl;


}

int memory::readMem(int address) {
    return program[address];
}

void memory::writeMem(int address, int data) {
    program[address] = data;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
