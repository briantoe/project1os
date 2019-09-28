//
// Created by briant on 9/15/19.
//


#include "cpu.h"

cpu::cpu(std::string input_file) {
    input.open(input_file);
}

void cpu::run() {

    while (!input.eof()) {
        this->fetch();
        this->execute();
    }
}

void cpu::fetch() {
//    cout << instruction << endl;
    input >> instruction;
    input.ignore(UINT_MAX, '\n');
}

void cpu::execute() {
    switch (instruction) {
        case 1:
            load_value();
        case 2:
            load_addr();
        case 3:
            loadInd_addr();
        case 4:
            loadIdxX_addr();
        case 5:
            loadIdxY_addr();
        case 6:
            loadSpX();
        case 7:
            store_addr();
        case 8:
            get();
        case 9:
            put_port();
        case 10:
            addx();
        case 11:
            addy();
        case 12:
            subx();
        case 13:
            suby();
        case 14:
            copy_to_x();
        case 15:
            copy_from_x();
        case 16:
            copy_to_y();
        case 17:
            copy_from_y();
        case 18:
            copy_to_sp();
        case 19:
            copy_from_sp();
        case 20:
            jump_addr();
        case 21:
            jump_if_equal();
        case 22:
            jump_not_equal();
        case 23:
            call_addr();
        case 24:
            ret();
        case 25:
            incx();
        case 26:
            decx();
        case 27:
            pushAC();
        case 28:
            popAC();
        case 29:
            syscall();
        case 30:
            retsyscall();
        case 50:
            end();
    }
}

void cpu::load_value() { // 1
    int tmp;
    input >> tmp;
    ac = tmp;

    input.ignore(UINT_MAX, '\n');
}

void cpu::load_addr() { // 2

}

void cpu::loadInd_addr() { // 3

}


void cpu::loadIdxX_addr() {

}

void cpu::loadIdxY_addr() {

}

void cpu::loadSpX() {

}

void cpu::store_addr() {

}

void cpu::get() { // 8
    srand(time(NULL));
    ac = rand() % 100 + 1;
}

void cpu::put_port() { // 9
    int tmp;
    input >> tmp;
    if (tmp == 1) {

    } else if (tmp == 2) {

    }

    input.ignore(UINT_MAX, '\n');
}

void cpu::addx() { // 10
    ac = ac + x_reg;
}

void cpu::addy() { // 11
    ac = ac + y_reg;
}

void cpu::subx() { // 12
    ac = ac - x_reg;
}

void cpu::suby() { // 13
    ac = ac - y_reg;
}

void cpu::copy_to_x() { // 14
    x_reg = ac;
}

void cpu::copy_to_y() { // 15
    y_reg = ac;
}

void cpu::copy_from_x() { // 16
    ac = x_reg;
}

void cpu::copy_from_y() { // 17
    ac = y_reg;
}

void cpu::copy_to_sp() {
    stack_pointer = ac;
}

void cpu::copy_from_sp() {
    ac = stack_pointer;
}

void cpu::jump_addr() {

}

void cpu::jump_if_equal() {

}

void cpu::jump_not_equal() {

}

void cpu::call_addr() {

}

void cpu::ret() {

}

void cpu::incx() {

}

void cpu::decx() {

}

void cpu::pushAC() {

}

void cpu::popAC() {

}

void cpu::syscall() {

}

void cpu::retsyscall() {

}

void cpu::end() {
    // more housekeeping needs to be done probably
exit(EXIT_SUCCESS);
}

