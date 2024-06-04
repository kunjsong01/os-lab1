#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find()
{
  int fd;
  struct dirent e;

  fd = open(".", 0);
  while(read(fd, &e, sizeof(e)) == sizeof(e)){
    if(e.name[0] != '\0'){
      printf("%s\n", e.name);
    }
  }
  exit(0);
}

int
main() {
  find();
  exit(0);
}
