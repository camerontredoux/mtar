#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

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
    printf("archive\n");
  } else if (strcmp(argv[1], "-x") == 0) {
    printf("extract\n");
  }
}
