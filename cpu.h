//
// Created by briant on 9/15/19.
//

#ifndef PROJECT1OSCPP_CPU_H
#define PROJECT1OSCPP_CPU_H

#include <string>
#include <fstream>
#include <climits>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <iostream>

class cpu{

public:
    int program_counter; // PC
    int stack_pointer; // SP
    int instruction_register; // IR
    int x_reg; // x register
    int y_reg; // y register
    int ac; // accumulator

    int instruction;
    int interrupt_timer;
    int timer;
    bool interrputed;
    bool kernelMode;

    cpu(int int_timer);
    ~cpu() {

    }
    int getPC();
    void incTimer();
    bool interrputTime();
    bool isInterrput();


    void load_value(int value); // load value into the ac (accumulator)
    void load_addr(); // load address of
    void loadInd_addr();
    void loadIdxX_addr(int addr); // Load the value at (address+X) into the AC
    //(for example, if LoadIdxX 500, and X contains 10, then load from 510).
    void loadIdxY_addr(int addr); // Load the value at (address+Y) into the AC
    void loadSpX(); // Load from(Sp+X) into the AC (if SP is 990 and X is 1, load from 991)
    void store_addr(); // store the value in the AC into the address
    void get(); // get a random int from 1 to 100 into the AC
    void put_port(int port); // if port = 1 write AC as an int to the screen
    // if port = 2, write AC as a char to the screen
    void addx(); // add the value X to the AC
    void addy(); // add the value Y to the AC
    void subx(); // sub the value in X from the AC
    void suby(); // sub the value in Y from the AC
    void copy_to_x(); // copy the value in AC to X
    void copy_to_y(); // copy the value in AC to Y
    void copy_from_x(); // copy value in X to AC
    void copy_from_y(); // copy value in Y to AC
    void copy_to_sp(); // copy value in AC to SP
    void copy_from_sp(); // copy value in SP toAC
    void jump_addr(int addr); // jump to address
    void jump_if_equal(int addr); // jump to address if equal
    void jump_not_equal(int addr); //jump to address if NOT equal
    void call_addr(int addr); // Push return address onto stack, jump to addr
    void ret(); // pop return address from stack, jump to address
    void incx(); // increment x
    void decx(); // decrement x
    int pushAC(); // push AC onto stack
    void popAC(int accum); // Pop from stack into AC
    void syscall(); // perform system call
    void retsyscall(int sp, int pc); // return from system call



};
#endif //PROJECT1OSCPP_CPU_H
