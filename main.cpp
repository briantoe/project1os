#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <fstream>
#include <climits>
#include "cpu.h"
#include "memory.h"


using namespace std;

const int EXIT_CODE = 9999;
const int HELP_WRITE = 3000;
const int HELP_FUNCTION_CALL = 4000;
const int HELP_RETURN = 6000;
const int HELP_PUSH = 7000;
const int HELP_POP = 8000;
const int HELP_INTERRUPT = 9000;
const int HELP_SYSTEM_RET = 124891;

int main(int argc, char **argv) {


    int pipe1[2]; // memory will use to write, cpu will use to read
    int pipe2[2]; // memory will use to read, cpu will use to write
    pid_t pid;

    int ret = pipe(pipe1); // pipe 0 is read, pipe 1 is write
    if (ret == -1) {
        cout << "unable to create pipes1" << endl;
        return 1;
    }
    ret = pipe(pipe2);
    if (ret == -1) {
        cout << "unable to create pipes2" << endl;
        return 1;
    }

    pid = fork();

    if (pid == 0) {// the child

        close(pipe1[0]); // close read end for memory on pipe 1
        close(pipe2[1]); // close write end for memory on  pipe 2
        // memory will use pipe2[0] for reading
        // memory will use pipe1[1] for writing

        // memory init code (child)
        memory m(argv[1]);
        int op = 0;
        while (true) {
//            int op = 0;

            if (op == EXIT_CODE) {
                cout << "memory exiting " << endl;
                return 0;
            } else if (op == HELP_WRITE) {
                int addr;
                int value;
                read(pipe2[0], &addr, sizeof(addr));
                read(pipe2[0], &value, sizeof(value));
                m.writeMem(addr, value);
            } else if (op == HELP_FUNCTION_CALL) {
                int retaddr;
                int stack_location;
                read(pipe2[0], &retaddr, sizeof(retaddr));
                read(pipe2[0], &stack_location, sizeof(stack_location));

                m.writeMem(stack_location, retaddr);
            } else if (op == HELP_RETURN) {
                int stack_location;
                read(pipe2[0], &stack_location, sizeof(stack_location));
                int retaddr = m.readMem(stack_location);
                // cout << "Reading from stack location: " << stack_location << endl;
                // cout << "Returning to address: " << retaddr << endl;
                // cout << "returning from function call " << endl;

                write(pipe1[1], &retaddr, sizeof(retaddr));
            } else if (op == HELP_PUSH) {
                int accum;
                int stack_location;
                read(pipe2[0], &accum, sizeof(accum));
                read(pipe2[0], &stack_location, sizeof(stack_location));

                m.writeMem(stack_location, accum);
            } else if (op == HELP_POP) {
                int accum;
                int stack_location;
                read(pipe2[0], &stack_location, sizeof(stack_location));
                accum = m.readMem(stack_location);
                write(pipe1[1], &accum, sizeof(accum));
            } else if (op == HELP_INTERRUPT) {
                // receive stack pointer and program counter to store on the system stack
//                int sysstack;
                int sp, pc;
//                read(pipe2[0], &sysstack, sizeof(sysstack));
                read(pipe2[0], &sp, sizeof(sp));
                read(pipe2[0], &pc, sizeof(pc));
                m.writeMem(1999, sp);
                m.writeMem(1998, pc);
//                int i = m.readMem(sysstack);
//                write(pipe1[1], &i, sizeof());
            } else if (op == HELP_SYSTEM_RET) {
                int sp = m.readMem(1999);
                int pc = m.readMem(1998);
                write(pipe1[1], &sp, sizeof(sp));
                write(pipe1[1], &pc, sizeof(pc));
            } else {
                int fetchedval = m.readMem(op);
                write(pipe1[1], &fetchedval, sizeof(fetchedval));
            }
            read(pipe2[0], &op, sizeof(op));
        }
    } else // the parent
    {
        close(pipe1[1]); // close write end for cpu on pipe 1
        close(pipe2[0]); // close read end for cpu on  pipe 2
        // cpu will use pipe1[0] for reading
        // cpu will use pipe2[1] for writing

        // cpu init code (parent)

        int timer = stoi(argv[2]);
        cpu c(timer);

        while (true) {
            // check if PC is accessing system memory illegally
            if (c.program_counter >= 1000 && !c.kernelMode) {
                cerr << "USER PROGRAM MAY NOT ACCESS SYSTEM MEMORY" << endl;
                write(pipe2[1], &EXIT_CODE, sizeof(EXIT_CODE));
                return -1;
            }


            int instruction;


            int addr;
            int value;

            // timer interrupt handler
            if (c.interrputTime() && !c.isInterrput()) {
                cout << "interrupted" << endl;
                c.kernelMode = true;
                const int SYSTEM = 1000;
                write(pipe2[1], &HELP_INTERRUPT, sizeof(HELP_INTERRUPT));
//                write(pipe2[1], &SYSTEM, sizeof(SYSTEM));
                write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));
                write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                write(pipe2[1], &SYSTEM, sizeof(SYSTEM));
            }
            read(pipe1[0], &instruction, sizeof(instruction));

//            cout << "Executing instruction: " << instruction     << endl;
            switch (instruction) {
                case 1: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &value, sizeof(value));
                    c.load_value(value);

                    break;
                }
                case 2: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    write(pipe2[1], &addr, sizeof(addr));
                    read(pipe1[0], &value, sizeof(value));
                    c.load_value(value);


                    break;
                }
                case 3: {
                    c.loadInd_addr();

                    break;
                }
                case 4: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    addr = addr + c.x_reg;
                    write(pipe2[1], &addr, sizeof(addr));
                    read(pipe1[0], &value, sizeof(value));
                    c.load_value(value);

                    break;
                }
                case 5: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    addr = addr + c.y_reg;
                    write(pipe2[1], &addr, sizeof(addr));
                    read(pipe1[0], &value, sizeof(value));
                    c.load_value(value);

                    break;
                }
                case 6: {
//                    c.loadSpX();
//                    read(pipe1[0], &addr, sizeof(addr));
                    addr = c.stack_pointer + c.x_reg;
                    write(pipe2[1], &addr, sizeof(addr));
                    read(pipe1[0], &value, sizeof(value));
                    c.load_value(value);

                    break;
                }
                case 7: {
//                    c.store_addr();
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    write(pipe2[1], &HELP_WRITE, sizeof(HELP_WRITE));

                    write(pipe2[1], &addr, sizeof(addr));
                    write(pipe2[1], &(c.ac), sizeof(c.ac));

                    break;
                }
                case 8: {
                    c.get();
                    // cout << "Random number: " << c.ac << endl;
                    break;
                }
                case 9: {
                    // cout << "in case 9" << endl;
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &value, sizeof(value));
                    c.put_port(value);


                    break;
                }
                case 10: {
                    c.addx();
                    break;
                }
                case 11: {
                    c.addy();
                    break;
                }
                case 12: {
                    c.subx();
                    break;
                }
                case 13: {
                    c.suby();
                    break;
                }
                case 14: {
                    c.copy_to_x();
                    break;
                }
                case 15: {
                    c.copy_from_x();
                    break;
                }
                case 16: {
                    c.copy_to_y();
                    break;
                }
                case 17: {
                    c.copy_from_y();
                    break;
                }
                case 18: {
                    c.copy_to_sp();
                    break;
                }
                case 19: {
                    c.copy_from_sp();
                    break;
                }
                case 20: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    c.jump_addr(addr);

//                    c.program_counter --;
                    break;
                }
                case 21: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    c.jump_if_equal(addr);
//                    c.program_counter --;
                    // cout << "in case 21: "<< endl;
                    break;
                }
                case 22: {
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));
                    c.jump_not_equal(addr);

                    break;
                }
                case 23: {
                    int addr;
                    c.program_counter += 1;
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));
                    read(pipe1[0], &addr, sizeof(addr));

                    int retaddr = c.program_counter + 1;
                    write(pipe2[1], &HELP_FUNCTION_CALL, sizeof(HELP_FUNCTION_CALL));
                    write(pipe2[1], &retaddr, sizeof(retaddr));
                    write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));
                    c.call_addr(addr);
                    c.program_counter--;
                    break;
                }
                case 24: {

                    int retaddr;
                    c.stack_pointer++;
                    write(pipe2[1], &HELP_RETURN, sizeof(HELP_RETURN));
                    write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));

                    read(pipe1[0], &retaddr, sizeof(retaddr));
                    c.program_counter = retaddr;

                    c.program_counter--;
                    break;
                }
                case 25: {
                    c.incx();
                    break;
                }
                case 26: {
                    c.decx();
                    break;
                }
                case 27: {
                    write(pipe2[1], &HELP_PUSH, sizeof(HELP_PUSH));
                    int accum = c.pushAC();
                    write(pipe2[1], &accum, sizeof(accum));
                    write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));
                    break;
                }
                case 28: {
                    int accum;
                    write(pipe2[1], &HELP_POP, sizeof(HELP_POP));
                    write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));
                    read(pipe1[0], &accum, sizeof(accum));
                    c.popAC(accum);
                    break;
                }
                case 29: {
                    const int SYSTEM = 1999;
                    write(pipe2[1], &HELP_INTERRUPT, sizeof(HELP_INTERRUPT));
                    write(pipe2[1], &SYSTEM, sizeof(SYSTEM));
                    write(pipe2[1], &(c.stack_pointer), sizeof(c.stack_pointer));
                    write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));

                    c.syscall();
                    break;
                }
                case 30: {
                    write(pipe2[1], &HELP_SYSTEM_RET, sizeof(HELP_SYSTEM_RET));
                    int sp, pc;
                    read(pipe1[0], &sp, sizeof(sp));
                    read(pipe1[0], &pc, sizeof(pc));

                    c.retsyscall(sp, pc);
                    break;
                }
                case 50: {
                    write(pipe2[1], &EXIT_CODE, sizeof(EXIT_CODE));
                    cout << "cpu exiting" << endl;
                    return 0;
                }
            }
            c.program_counter += 1;
            write(pipe2[1], &(c.program_counter), sizeof(c.program_counter));

            if (!c.kernelMode)
                c.incTimer();

        }
    }

}
