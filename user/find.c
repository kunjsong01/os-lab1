#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char *target)
{
  int fd;
  struct dirent e;

  fd = open(".", 0);
  while(read(fd, &e, sizeof(e)) == sizeof(e)){
    if(e.name[0] != '\0') {
      // recursive call if found a directory
      struct stat st;
      int fdDirTest;
      char buf[512], *p;

      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      memmove(p, e.name, DIRSIZ); // new path
      p[DIRSIZ] = 0;
      printf("Now checking path: %s\n", buf);
      // now buf should contain the path

      if(strlen(p) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }

      if((fdDirTest = open(buf, 0)) < 0){
        printf("find: cannot open %s\n", buf);
        continue;
      }

      if(fstat(fdDirTest, &st) < 0){
        printf("find: cannot fstat %s\n", buf);
        close(fdDirTest);
        continue;
      }

      if(st.type == T_DIR) {
        printf("Found dir! - %s\n", e.name);
      }
      else if(st.type == T_FILE || st.type == T_DEVICE) {
        printf("Found non-dir! - %s\n", e.name);
      }
    }

    if(e.name[0] != '\0' && strcmp(target, e.name) == 0){
      printf("Got it! - %s\n", e.name);
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
