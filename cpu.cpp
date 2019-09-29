//
// Created by briant on 9/15/19.
//


#include "cpu.h"

cpu::cpu(int int_timer) {

    interrupt_timer = int_timer;
    stack_pointer = 999;
    x_reg = 0;
    y_reg = 0;
    program_counter = 0;
    timer = 0;
    interrputed = 0;
    kernelMode = 0;
}

void cpu::incTimer() {
    timer += 1;
}
bool cpu::interrputTime() {

    return ((timer % interrupt_timer) == 0 && (timer != 0));
}
bool cpu::isInterrput() {
    return interrputed;
}

void cpu::load_value(int value) { // 1
    ac = value;

//    printf("Loaded value: %d", ac);
}

void cpu::load_addr() { // 2

}

void cpu::loadInd_addr() { // 3

}


void cpu::loadIdxX_addr(int addr) { // 4
    ac = addr + y_reg;
}

void cpu::loadIdxY_addr(int addr) { // 5
    ac = addr + y_reg;
}

void cpu::loadSpX() {
    int addr = stack_pointer + x_reg;
}

void cpu::store_addr() {

}

void cpu::get() { // 8
    srand(time(NULL));
    ac = rand() % 100 + 1;
}

void cpu::put_port(int port) { // 9
//    if(ac == 3){
//        printf("\nLast Number\n");
//    }
    if (port == 1) {
        printf("%d", ac);
    } else if (port == 2) {
        printf("%c", ac);
    }

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

void cpu::jump_addr(int addr) {
    program_counter = addr - 1;
}

void cpu::jump_if_equal(int addr) {
    if (ac == 0)
        program_counter = addr - 1;
}

void cpu::jump_not_equal(int addr) {
    if (ac != 0)
        program_counter = addr - 1;
}

void cpu::call_addr(int addr) {
    // adding to stack is done outside of this function

    stack_pointer--;
    program_counter = addr;
}

void cpu::ret() {
    // no need to remove from stack, just mark it as clear.
    // read the return address from the stack
    stack_pointer++;
//    program_counter = addr;
    // program_counter will be set outside of this function
}

void cpu::incx() {
    x_reg++;
}

void cpu::decx() {
    x_reg--;
}

int cpu::pushAC() {
    stack_pointer--;
    return ac;
}

void cpu::popAC(int accum) {
    ac = accum;
    stack_pointer++;
}

void cpu::syscall() {

    interrputed = !interrputed;
    program_counter = 1500 - 1;
    kernelMode = true;
}


void cpu::retsyscall(int sp, int pc) {
    interrputed = !interrputed;
    stack_pointer = sp;
    program_counter = pc;
    kernelMode = false;
}

int cpu::getPC() {
    return program_counter;
}
