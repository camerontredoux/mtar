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

struct tarFILE {
  const char *fileName;
  long permissions;
  long long a_timestamp;
  long long c_timestamp;
  long long m_timestamp;
  void *content;
};

tarFILE *archive(const char *fileName) {
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
  tarFILE *tfptr = (tarFILE *)malloc(sizeof(tarFILE));
  if (S_ISREG(sb.st_mode)) {
    tfptr->fileName = fileName;
    tfptr->permissions = sb.st_mode;
    tfptr->a_timestamp = sb.st_atim.tv_sec;
    tfptr->c_timestamp = sb.st_ctim.tv_sec;
    tfptr->m_timestamp = sb.st_mtim.tv_sec;
  }
  fclose(fptr);
  return tfptr;
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

  vector<tarFILE *> tar_files;
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
    FILE *fptr = fopen(argv[2], "w");
    if (!fptr) {
      perror("fopen()");
      exit(1);
    }
    for (int i = 3; i < argc; i++) {
      tarFILE *current_file = archive(argv[i]);
      tar_files.push_back(current_file);
    }
    int bytes_written = fwrite(&tar_files, sizeof(tarFILE),
                               tar_files.size() * sizeof(tarFILE), fptr);
    printf("%d\n", bytes_written);
    fclose(fptr);
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
  for (tarFILE *t : tar_files) {
    printf("%s\n", t->fileName);
    free(t);
  }

  FILE *fptr = fopen(argv[2], "r");
  if (!fptr) {
    perror("fopen()");
    exit(1);
  }
  vector<tarFILE *> read_files;
  fseek(fptr, 0, SEEK_END);
  int size = ftell(fptr);
  rewind(fptr);
  int bytes_read = fread(&read_files, sizeof(tarFILE), size, fptr);
  printf("%d\n", bytes_read);
  // for (tarFILE *f : tar_files) {
  //   printf("f:%s\n", f->fileName);
  // }
  fclose(fptr);
}
