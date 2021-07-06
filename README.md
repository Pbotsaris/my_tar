# My tar

Under construction...


## Dev options 
Option `-d <filename>` for debug. While on debug mode, the program will skip all archiving and print out a ASCII representation of the header produced from `<filename>`. Example: 


    ./bin/my_tar -d 

output:

    Field: name -- length: 100  -- offset: 0
        |  t  |  e  |  s  |  t  |  _  |  f  |  i  |  l  |  e  |  s  |  /  |  f  |  .  |  t  |  x  |  t  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _
    |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _
        |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _  |  _
   |  _  |
      (...)

where `field` is the name of the field,  `|   |` represents a memory block of 1 byte, `length` is the length of a field, `offset` is the memory offset of a field in within the header 512 bytes block and `---` is a divider between fields.

## Testing

The testing script will run `my_tar` in skip mode and the real `tar` then cat the outputs for comparison. First, update permissions:

    $ chmod u+x bin/test.sh

Run the script:

    $ ./bin/test.sh <filename>

For debug mode pass the `-d` option 

    $ ./bin/test.sh -d <filename>
