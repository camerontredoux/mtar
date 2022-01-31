# Homework 04: mTar

## Requirements:

- Must support two command line options

  - -a file.mtar file1 [files...]
  - -x file.mtar

- Options, explained:

  - **-a file.mtar file1 [files...]**
    - -a: "archive"
    - file.mtar: name of archive file
    - file1: must have at least one file to archive
    - files...: optional additional files
  - **-x file.mtar**
    - -x: "extract"
    - file.mtar: archived file from previous execution

- Archiving:

  - Must combine all file attributes, contents into single archival file (.mtar)
    - filename
    - permissions
    - timestamps
    - contents
  - Must work with any type of regular file
    - text
    - binary
    - jpeg
    - pdf
  - Must ignore other types of files
    - directories
    - sockets
  - S_ISREG() to determine file type

- Extraction:
  - Must take single archive file (.mtar)
    - .mtar archive file generated using same program (with -a option)
  - Must recreate and fully restore all files in archive file
    - filename
    - timestamps
    - permissions
    - contents

---

### Hints

- chmod
- utime
- utimbuf
