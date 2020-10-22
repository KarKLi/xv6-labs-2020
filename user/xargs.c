#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char **argv) {
  char args[MAXARG][32];
  char *p[MAXARG];
  char buf;
  int i;
  int j;
  if (argc < 2)
    exit(0);
  // xargs is a user program that can read params from stdin to execute the
  // command.
  while (1) {
    // First, copy original args
    memset(args, 0, MAXARG * 32 * sizeof(char));
    i = 0;
    for (j = 1; j < argc; j++)
      strcpy(args[i++], argv[j]);
    while (i < MAXARG - 1) {
      j = 0;
      // Read from stdin
      if (read(0, &buf, 1) <= 0) {
        // Ctrl+D
        wait((int *)0);
        exit(0);
      }
      if (buf == '\n')
        break;
      // If there is a blank, switch to new string
      if (buf == ' ') {
        j = 0;
        i++;
      } else {
        args[i][j++] = buf;
      }
    }
    // Copy to p
    for (i = 0; i < MAXARG - 1; i++)
      p[i] = args[i];
    p[MAXARG - 1] = 0;
    if (fork() == 0) {
      exec(argv[1], p);
      exit(0);
    }
  }
  exit(0);
}