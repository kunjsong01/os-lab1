#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipe1[2];
  int pipe2[2];

  pipe(pipe1);
  pipe(pipe2);

  int pid;

  pid = fork();

  if(pid == 0) {
    // child
    char buf[1];

    // pipe1 in child: child reads
    close(0);
    dup(pipe1[0]);
    close(pipe1[0]);
    close(pipe1[1]);
    if(read(0, buf, sizeof(buf))) {
      printf("%d: received ping\n", getpid());
    }

    // pipe2 in child: child writes
    close(pipe2[0]);
    write(pipe2[1], buf, sizeof(buf));
    close(pipe2[1]);

    exit(0);
  }
  else {
    // parent
    char buf[1];
    // pipe1 in parent: parent writes
    close(pipe1[0]);
    write(pipe1[1], buf, sizeof(buf));
    close(pipe1[1]);
    wait((int*)0); // wait for child to write

    // pipe2 in parent: parent reads
    close(0);
    dup(pipe2[0]);
    close(pipe2[0]);
    close(pipe2[1]);
    if(read(0, buf, sizeof(buf))) {
      printf("%d: received pong\n", getpid());
    }

    exit(0);
  }

  exit(0);
}
