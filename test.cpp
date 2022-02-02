#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
using namespace std;

int main() {
  // create file
  FILE *t = fopen("zero.mtar", "w");

  // get stats on "one"
  struct stat sb;
  // open one for reading text
  FILE *t1 = fopen("one", "r");
  FILE *t2 = fopen("two", "r");

  int count = 0;
  stat("one", &sb);
  printf("%lu", sb.st_size);

  char c[sb.st_size];
  int bytes_read = fread(c, sizeof(char), sb.st_size, t1);

  fwrite(&sb, sizeof(struct stat), 1, t);
  fwrite(c, sizeof(char), sb.st_size, t);

  stat("two", &sb);
  printf("%lu", sb.st_size);
  char s[sb.st_size];
  fread(s, sizeof(char), sb.st_size, t2);
  fwrite(&sb, sizeof(struct stat), 1, t);
  fwrite(s, sizeof(char), sb.st_size, t);
  // tf.content[31] = '\0';
  // printf("%s\n", tf.content);
  // stat("two", &sb);
  // fwrite(&sb, sizeof(struct stat), 1, t);

  fclose(t);
}
