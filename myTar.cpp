#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

#ifdef __APPLE__
#define st_atim st_atimespec
#define st_ctim st_ctimespec
#define st_mtim st_mtimespec
#endif

void help()
{
  printf("myTar:\t-a: archive\n\t\tfile.mtar: name of archive file\n\t\tfile1: "
         "must have at least one file to "
         "archive\n\t\tfiles: optional additional files to archive\n\t-x: "
         "extract\n\t\tfile.mtar: archived file to extract files from\n\n");
}

void usage_err()
{
  fprintf(stderr, "myTar:\tarchive: ./myTar -a file.mtar file1 "
                  "[files...]\n\textract: ./myTar -x file.mtar\n\n");
}

void file_err(const char *type)
{
  fprintf(stderr, "myTar: \t%s: incorrect file extension\n\n", type);
}

void check_extension(const char *fileName)
{
  const char *fileNameExtension = strrchr(fileName, '.');
  if (!fileNameExtension || strcmp(fileNameExtension, ".mtar") != 0)
  {
    file_err("archive");
    exit(1);
  }
}

char *read_file(FILE *fptr, off_t size)
{
  char *file_content = (char *)malloc(sizeof(char) * size);
  if (fread(file_content, sizeof(char), size, fptr) < size)
  {
    perror("fread() file content");
    exit(1);
  }
  file_content[size] = '\0';
  return file_content;
}

void archive(const char *fileName, FILE *mtarptr)
{
  FILE *fptr = fopen(fileName, "r");
  if (!fptr)
  {
    perror("fopen()");
    exit(1);
  }

  struct stat sb;
  if (stat(fileName, &sb) == -1)
  {
    perror("stat()");
    exit(1);
  }

  if (S_ISREG(sb.st_mode))
  {
    int fileName_length = strlen(fileName);
    fwrite(&fileName_length, sizeof(int), 1, mtarptr);
    fwrite(&sb, sizeof(struct stat), 1, mtarptr);
    fwrite(fileName, sizeof(char), fileName_length, mtarptr);

    char *file_content = read_file(fptr, sb.st_size);
    fwrite(file_content, sizeof(char), sb.st_size, mtarptr);

    free(file_content);
    fclose(fptr);
  }
  else
  {
    fprintf(
        stderr,
        "myTar:\tarchive: incorrect file type (must be a regular file)\n\n");
    exit(1);
  }
}

void extract(const char *mtarname)
{
  FILE *mtarptr = fopen(mtarname, "r");
  if (!mtarptr)
  {
    perror("fopen()");
    exit(1);
  }

  struct stat sb;

  int fileName_length;
  if (fread(&fileName_length, sizeof(int), 1, mtarptr) < 1)
  {
    perror("fread() filename length");
    exit(1);
  }
  if (fread(&sb, sizeof(struct stat), 1, mtarptr) < 1)
  {
    perror("fread() stat buffer");
    exit(1);
  }
  char *fileName = read_file(mtarptr, fileName_length);
  char *file_content = read_file(mtarptr, sb.st_size);

  FILE *fptr = fopen(fileName, "w");
  if (!fptr)
  {
    perror("fopen() create file");
    exit(1);
  }
  fwrite(file_content, sizeof(char), sb.st_size, fptr);

  char c;
  c = fgetc(fptr);
  printf("%c\n", c);
  if (feof(fptr))
  {
    extract(mtarname);
  }

  fclose(mtarptr);
  fclose(fptr);
  free(fileName);
  free(file_content);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    usage_err();
    exit(1);
  }

  /* ARCHIVE */
  if (strcmp(argv[1], "-a") == 0)
  {
    if (argc < 4)
    {
      fprintf(stderr,
              "myTar:\tarchive: ./myTar -a file.mtar file1 [files...]\n\n");
      exit(1);
    }

    check_extension(argv[2]);

    FILE *mtarptr = fopen(argv[2], "w");
    if (!mtarptr)
    {
      perror("fopen()");
      exit(1);
    }
    for (int i = 3; i < argc; i++)
    {
      archive(argv[i], mtarptr);
    }

    fclose(mtarptr);
  }
  /*EXTRACT*/
  else if (strcmp(argv[1], "-x") == 0)
  {
    if (argc < 3)
    {
      fprintf(stderr, "myTar:\textract: ./myTar -x file.mtar\n\n");
      exit(1);
    }

    check_extension(argv[2]);

    FILE *tarptr = fopen(argv[2], "r");
    if (!tarptr)
    {
      perror("fopen()");
      exit(1);
    }
    extract(argv[2]);
  }
  /* HELP */
  else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
  {
    help();
    exit(0);
  }
  /* ERROR */
  else
  {
    usage_err();
    exit(1);
  }
}
