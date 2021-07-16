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

There is a very basic script to help test some of this programs functionalities. First, make sure you set the permissions accordingly: 

    $ chmod u+x bin/test.sh

Run test:

    $ ./bin/test.sh 

This command will run the program in all its available options consecutively. Appending with `-u` is not tested in this mode but using the `--append` option. The script will test **My Tar** against all 
file types in the `test_files`. This repo includes fifo, symlink, hardlink, regular and directory file types in `test_files`. 

The script will clean up after itself removing all temp files.

the script will compile with make before attempting to run the program.

Other available options:

    $ ./bin/test.sh --cat file_to_archive ...

`--cat` will archive a file in both `my_tar` and the original `tar` and `cat` their contents for comparisson. OSX users must install `gtar` to run the script.
Darwin uses bdstar as its default`tar`and `gtar` ensures we get a consitent tar format.


    $ ./bin/test.sh -d file_to_header

Runs the program in debug mode. While in debug mode no files will be archived but the program will output the contents of the header this passed in file generates. Only 1 file can be debug at the time. 

    $ ./bin/test.sh --append tar_file_to_apend.tar file_to_append

Use `--append` for testing th `=uf` option. Remember that you must pass in an existing file in the archive in other to append using `-u`.


If you have valgrind and want memory check: 

    $ ./bin/test.sh --append tar_file_to_apend.tar file_to_append


