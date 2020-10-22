#include "kernel/types.h"
#include "user/user.h"

int isPrime(int num)
{
    int i = 2;
    for (;i < num; i++)
    {
        if (num % i == 0) return 1;
    }
    return 0;
}

void recursive()
{
    // Read from parent pipe
    // Because first element 2 is prime, so we can read it directly
    int p[2];
    int num;
    int n;
    // There will block before child process finish its first data writing job.
    // Notice that fd 0 link to new-created R-pipe.
    // Parent process will read child process's written data.
    if (read(0,&num,sizeof(num))) {
        printf("prime %d\n",num);
        pipe(p);
        int pid = fork();
        if (pid > 0)
        {
            // parent process
            // Parent process only read data by the R-pipe fd.
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            recursive();
        } else if (pid == 0){
            // child process
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            // Parent process close fd 0 will not affect child process.
            // fork() copys all of the fd(s), making child process can operate fd 1
            // separately.
            // Now we see, child process only will write data by the W-pipe fd.
            // There is read from left pipe, and write to right pipe.
            // Notice that fd 0 still remains the previous R-pipe, but
            // fd 1 has become new-created W-pipe.
            // Notice that there is only fd 0, fd 1, fd p[0], fd p[1] be used, so
            // there will NEVER have fd exhaust problem.
            // We call it fd multiplexing.
            while(read(0,&n,sizeof(n)))
            {
                if (n % num)
                    write(1,&n,sizeof(n));
            }
        } else {
            fprintf(2,"fork() failed. From process(pid): %d\n",getpid());
        }
    }
}
int main(int argc, char **argv)
{
    // First, create pipe
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid > 0) {
        // parent process
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        recursive();
    } else if(pid == 0) {
        // child process
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        int i = 2;
        for(;i<=35;i++) write(1,&i,sizeof(i));
    } else {
        fprintf(2,"fork() failed. From process(pid): %d\n",getpid());
    }
    exit(0);
}