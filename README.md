# My tar

Under construction...


## Dev options 
Use `-d` for debug mode and `-s` to skip parsing options to avoid errors.

Debug:

    ./bin/my_tar -d 

Skip:

    ./bin/my_tar -s 

Both:

    ./bin/my_tar -d -s

## Testing

The testing script will run `my_tar` in skip mode and the real `tar` then cat the outputs for comparison. First, update permissions:

    $ chmod u+x bin/test.sh

Run the script:

    $ ./bin/test.sh

For debug mode pass the `-d` option 

    $ ./bin/test.sh -d
