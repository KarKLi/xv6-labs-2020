#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char ** argv)
{
    // First, let's create child pipe and parent pipe
    int parent_fd[2];
    int child_fd[2];
    char buf[64];
    pipe(parent_fd);
    pipe(child_fd);
    if (fork() == 0) {
        // Child process
        // Child read from parent pipe
        read(parent_fd[0],&buf,4);
        printf("%d: received %s\n",getpid(),buf);
        write(child_fd[1],"pong",strlen("pong"));
    } else {
        // Parent process
        write(parent_fd[1],"ping",strlen("ping"));
        read(child_fd[0],&buf,4);
        printf("%d: received %s\n",getpid(),buf);
    }
    exit(0);
}