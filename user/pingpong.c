#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;

  pid = fork();

  if(pid == 0) {
    printf("in child process\n");
  }
  else if(pid > 0) {
    printf("in parent process, before wait\n"); // interleaved
    pid = wait((int*) 0);
    printf("in parent process, child finished\n");
  }
  else {
    printf("failed to create child process\n");
  }

  exit(0);
}
