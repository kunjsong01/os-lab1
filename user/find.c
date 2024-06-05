#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *target)
{
  int fd;
  struct dirent e;

  fd = open(".", 0);
  while(read(fd, &e, sizeof(e)) == sizeof(e)){
    if(e.name[0] != '\0' && strcmp(target, e.name) == 0){
      printf("%s\n", e.name);
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "usage: find [path] [filename]\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
