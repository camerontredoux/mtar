#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void help() {
  printf("myTar:\t-a: archive\n\t\tfile.mtar: name of archive file\n\t\tfile1: "
         "must have at least one file to "
         "archive\n\t\tfiles: optional additional files to archive\n\t-x: "
         "extract\n\t\tfile.mtar: archived file to extract files from\n\n");
}

void usage_err() {
  fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "
                  "[files...]\n\textract: ./myTar -x file.mtar\n\n");
}

void file_err(const char *type) {
  fprintf(stderr, "myTar: \t%s: incorrect file extension\n\n", type);
}

void archive(const char *fileName) {
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
    file_err("archive");
    exit(1);
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
    exit(1);
  }
  FILE *fptr = fopen(fileName, "r");
  if (fptr) {
    fclose(fptr);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage_err();
    exit(1);
  }

  if (strcmp(argv[1], "--help") == 0) {
    help();
    exit(0);
  }

  if (strcmp(argv[1], "-a") == 0) {
    if (argc < 3) {
      fprintf(stderr,
              "myTar:\tarchive: ./myTar -a file.mtar file1 [files...]\n\n");
      exit(1);
    }
    archive(argv[2]);
  } else if (strcmp(argv[1], "-x") == 0) {
    if (argc < 3) {
      fprintf(stderr, "myTar:\textract: ./myTar -x file.mtar\n\n");
      exit(1);
    }
    extract(argv[2]);
  } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    help();
    exit(0);
  } else {
    usage_err();
    exit(1);
  }
}
