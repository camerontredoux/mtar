#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

#define help()                                                                 \
  printf("myTar:\t-a: archive\n\t\tfile.mtar: name of archive "                \
         "file\n\t\tfile1: must have at least one file to "                    \
         "archive\n\t\tfiles: optional additional files to archive\n\t-x: "    \
         "extract\n\t\tfile.mtar: archived file to extract files from\n\n");

#define usage_err()                                                            \
  fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "               \
                  "[files...]\n\textract: ./myTar -x file.mtar\n\n");

#define file_err(TYPE)                                                         \
  fprintf(stderr, "myTar: \t%s: incorrect file extension\n\n", TYPE);

void archive(const char *fileName) {
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
    file_err("archive");
    exit(EXIT_FAILURE);
  }
  FILE *fptr = fopen(fileName, "w");
  struct stat sb;
  stat(fileName, &sb);
  if (S_ISREG(sb.st_mode)) {
    printf("Can be archived\n");
  }
}

void extract(const char *fileName) {
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
    file_err("extract");
    exit(EXIT_FAILURE);
  }
  FILE *fptr = fopen(fileName, "r");
  if (fptr) {
    fclose(fptr);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage_err();
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "--help") == 0) {
    help();
    exit(EXIT_SUCCESS);
  }

  if (argv[1][0] == '-') {
    switch (argv[1][1]) {
    case 'a':
      if (argc < 3) {
        fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "
                        "[files...]\n\n");
        exit(EXIT_FAILURE);
      }
      archive(argv[2]);
      break;
    case 'x':
      if (argc < 3) {
        fprintf(stderr, "myTar:\textract: ./myTar -x file.mtar\n\n");
        exit(EXIT_FAILURE);
      }
      extract(argv[2]);
      break;
    case 'h':
      help();
      exit(EXIT_SUCCESS);
      break;
    default:
      usage_err();
      exit(EXIT_FAILURE);
    }
  }
}
