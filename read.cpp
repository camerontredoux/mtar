#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
using namespace std;

struct tar {
  struct stat sb;
  const char *content;
};

int main() {
  struct stat sb;
  FILE *t = fopen("zero.mtar", "r");

  fread(&sb, sizeof(struct stat), 1, t);
  printf("%lu\n", sb.st_size);
  char c[sb.st_size];
  fread(c, sizeof(char), sb.st_size, t);
  c[sb.st_size] = '\0';
  printf("%s", c);

  fread(&sb, sizeof(struct stat), 1, t);
  printf("%lu\n", sb.st_size);
  char s[sb.st_size];
  fread(s, sizeof(char), sb.st_size, t);
  s[sb.st_size] = '\0';
  printf("%s", s);

  fclose(t);
}
