#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

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

char *read_file(FILE *fptr, off_t size) {
  char *file_content = (char *)malloc(sizeof(char) * size);
  int bytes_read = fread(file_content, sizeof(char), size, fptr);
  if (bytes_read < 0) {
    perror("fread()");
    exit(1);
  }
  return file_content;
}

void archive(const char *fileName, FILE *tarptr) {
  FILE *fptr = fopen(fileName, "r");
  if (!fptr) {
    perror("fopen()");
    exit(1);
  }

  struct stat sb;
  if (stat(fileName, &sb) == -1) {
    perror("stat()");
    exit(1);
  }

  if (S_ISREG(sb.st_mode)) {
    printf("%lu\n", strlen(fileName));
    printf("%s\n", fileName);

    fwrite(&sb, sizeof(struct stat), 1, tarptr);
    fwrite(fileName, sizeof(char), strlen(fileName) + 1, tarptr);

    char *file_content = read_file(fptr, sb.st_size);
    fwrite(file_content, sizeof(char), sb.st_size, tarptr);
    free(file_content);

    fclose(fptr);
  } else {
    fprintf(
        stderr,
        "myTar:\tarchive: incorrect file type (must be a regular file)\n\n");
    exit(1);
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

  /* ARCHIVE */
  if (strcmp(argv[1], "-a") == 0) {
    if (argc < 3) {
      fprintf(stderr,
              "myTar:\tarchive: ./myTar -a file.mtar file1 [files...]\n\n");
      exit(1);
    }

    const char *fileNameExtension = strrchr(argv[2], '.');
    if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0) {
      file_err("archive");
      exit(1);
    }

    FILE *tarptr = fopen(argv[2], "w");
    if (!tarptr) {
      perror("fopen()");
      exit(1);
    }
    for (int i = 3; i < argc; i++) {
      archive(argv[i], tarptr);
    }

    fclose(tarptr);
  }
  /*EXTRACT*/
  else if (strcmp(argv[1], "-x") == 0) {
    if (argc < 3) {
      fprintf(stderr, "myTar:\textract: ./myTar -x file.mtar\n\n");
      exit(1);
    }
    extract(argv[2]);
  }
  /* HELP */
  else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    help();
    exit(0);
  }
  /* ERROR */
  else {
    usage_err();
    exit(1);
  }
}
