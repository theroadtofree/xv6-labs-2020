#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_MAX 16

int main(int argc, char *argv[])
{
    if(argc != 1){
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    int pid;
    int pipes1[2], pipes2[2];
    char buf[BUF_MAX] = {0};
    pipe(pipes1);
    pipe(pipes2);

    int ret = fork();

    // parent send in pipes1[1], child receives in pipes1[0]
    // child send in pipes2[1], parent receives in pipes2[0]
    // should have checked close & read & write return value for error
    if (ret == 0) {
        // i am the child
        pid = getpid();
        close(pipes1[1]);
        close(pipes2[0]);

        read(pipes1[0], buf, BUF_MAX);
        printf("child %d: received ping %s\n", pid, buf);
        char sendbuf[BUF_MAX] = "child";
        write(pipes2[1], sendbuf, BUF_MAX);
        exit(0);
    } else {
        // i am the parent
        pid = getpid();
        close(pipes1[0]);
        close(pipes2[1]);
        char sendbuf[BUF_MAX] = "parent";
        write(pipes1[1], sendbuf, BUF_MAX);
        read(pipes2[0], buf, BUF_MAX);
        printf("parent %d: received pong %s\n", pid, buf);
        exit(0);
    }

    exit(0);
}
