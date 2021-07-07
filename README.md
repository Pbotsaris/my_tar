# My tar

Under construction...


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
