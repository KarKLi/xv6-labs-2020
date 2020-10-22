#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    // Return ASAP while there is no parameter.
    if (argc != 2) {
        fprintf(2,"sleep: wants 1 parameters, get %d parameter(s).\n",argc-1);
        exit(1);
    }
    int second;
    second = atoi(argv[1]);
    // If second == 0, return ASAP.
    if (second <= 0) exit(1);
    sleep(second);
    exit(0);
}