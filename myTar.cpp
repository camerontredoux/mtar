#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

void archive(const char *fileName) {
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
    fprintf(stderr, "myTar: \tarchive: incorrect file extension\n\n");
    exit(EXIT_FAILURE);
  }
}

void extract(const char *fileName) {
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
    fprintf(stderr, "myTar: \textract: incorrect file extension\n\n");
    exit(EXIT_FAILURE);
  }
  FILE *fptr = fopen(fileName, "w");
  if (fptr) {
    fclose(fptr);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "
                    "[files...]\n\textract: ./myTar -x file.mtar\n\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "--help") == 0) {
    printf("myTar:\t-a: archive\n\t\tfile.mtar: name of archive "
           "file\n\t\tfile1: must have at least one file to "
           "archive\n\t\tfiles: optional additional files to archive\n\t-x: "
           "extract\n\t\tfile.mtar: archived file to extract files from\n\n");
  } else if (strcmp(argv[1], "-a") == 0) {
    if (argc < 3) {
      fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "
                      "[files...]\n\n");
      exit(EXIT_FAILURE);
    }
    archive(argv[2]);
  } else if (strcmp(argv[1], "-x") == 0) {
    if (argc < 3) {
      fprintf(stderr, "myTar:\textract: ./myTar -x file.mtar\n\n");
      exit(EXIT_FAILURE);
    }
    extract(argv[2]);
  }
}
