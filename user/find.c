#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *fileName) {
  printf("find: searching %s recursively in %s\n", fileName, path);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  char* cwd = ".";
  char* pd = "..";

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
    case T_DEVICE:
    case T_FILE:
      if (strcmp(fmtname(path), fmtname(fileName)) == 0)
      {
        printf("%s\n", fmtname(path));
      }
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';

      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) {
          continue;
        }
        printf("find: Got %s\n", fmtname(de.name));
        if(strcmp(fmtname(de.name), fmtname(cwd)) == 0 || strcmp(fmtname(de.name), fmtname(pd)) == 0) {
          continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
          printf("find: cannot stat %s\n", buf);
          continue;
        }

        find(buf, fileName);
      }
      break;
  }
}

int main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find [dir] [file name]\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}