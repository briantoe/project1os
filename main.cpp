#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <climits>
#include "cpu.h"
#include "memory.h"


using namespace std;

int main(int argc, char** argv)
{

    memory m(argv[1]);
    m.print();
// child will be memory, parent will be cpu

/*
        pid_t  pid;
        int mypipefd[2];
        int ret;
        char buf[5];

        ret = pipe(mypipefd);

        if(ret != 0)
        {
            perror("pipe");
            exit(1);
        }

        pid = fork();

        if(pid == 0)
        {
            // child process
            printf("child process with pid: %d\n", (int)getpid());
            int n = 22;
            write(mypipefd[1], &n, sizeof(n));
        }
        else{
            // parent process
            printf("parent process with pid: %d\n", (int)getpid());
            int n;
            read(mypipefd[0], &n, sizeof(n));
            printf("buf: %d\n" , n+1);
        }
*/




//    ifstream input;
//    input.open(argv[1]);
//    cout << input.good() << endl;
//
//    int instruction;
//    while(input >> instruction)
//    {
//
//        cout << instruction << endl;
//
//        input.ignore(UINT_MAX, '\n');
//
//    }
}
