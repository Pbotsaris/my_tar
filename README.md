# My tar
A Naive implementation of the legendary Tape Archiver. 

## Build
While in the project root directory, run `make` to build the binary:

    $ make

You can also run `clean` to removed '/bin/my_tar' and `.o` artifacts.

    $ make clean

## Usage 

    $ .bin/my_tar [-cutx] [-f file.tar] file_to_archive ...

## Options
- `-cf file.tar file_to_archive ...` : Archive a file(s) or directories.  Multiple files is possible.
- `-uf file.tar file_to_archive ...` : Updates archive with new files.
- `-tf file.tar` :  Lists files in the tape archive.
- `-xf file.tar` :  Extracts all files in a archive. Existing files in the disk will be overwritten

When `-cf` directories, My tar will archive all files within the directory. Sub directories will be tar'ed as directories but their contents will not be included.

## Dev options 
Option `-d <filename>` for debug. While on debug mode, the program will skip all archiving and print out a ASCII representation of the header produced from `<filename>`. Example: 


    ./bin/my_tar -d <filename>

example output:

     Field: chksum -- length: 8  -- offset: 148
     |  0  |  1  |  3  |  2  |  0  |  0  |     |  _  |
        
     --
        
     Field: magic -- length: 6  -- offset: 257
     |  u  |  s  |  t  |  a  |  r  |  _  |
        
      (...)

where
- `|   |` represents a memory block of 1 byte. And empty block is an `<space>` char. 
- `| _ |` is an empty block
- `---` is a divider between fields.
- `field` is the name of the field 
- `length` is the length of a field
- `offset` is the memory offset of a field in within the header 512 bytes block and

## Testing

The testing script will run `my_tar` in skip mode and the real `tar` then cat the outputs for comparison. First, update permissions:

    $ chmod u+x bin/test.sh

Run the script:

    $ ./bin/test.sh <filename>

For debug mode pass the `-d` option 

    $ ./bin/test.sh -d <filename>
