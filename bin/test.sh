#!/bin/bash
echo " "

run_real_tar(){
  if [[ "$OSTYPE" == "darwin"* ]]; then
    gtar -cf real.tar $@
  elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    tar -cf real.tar $@
  fi
}

# run make first
echo "** Running Make to compile.."
make
echo "--"
echo " "

ITER=1 

if [[ -z $1 ]]; then
  for file in test_files/*; do

    echo " "
    echo "** Testing " "$file"
    echo "--"
    echo " "
    echo "Archiving:"
   ./bin/my_tar -cf t.tar "$file"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Appending same file:"
   ./bin/my_tar -uf t.tar "$file"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Appending another file:"
    echo "another file to append" > test_files/append.txt
   ./bin/my_tar -uf t.tar test_files/append.txt
    rm test_files/append.txt
    echo "-"
  
    echo "List all files files:"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Creating output folder for extraction:"
    mkdir output
    mv t.tar output/t.tar
    ls -la output
    echo "-"

    echo "Extracting files:"
   ./bin/my_tar -xf output/t.tar
    echo "-"

    echo "Listing output folder:"
    ls -la output
    echo "-"

    echo "Removing output folder"
    rm -rf output

  done

else

for arg in "$@"; do

  ITER=$(expr $ITER + 1)
  if [[ $arg == "-d" ]]; then

    ./bin/my_tar "$arg" "${@:$ITER}"

  elif [[ $arg == "--cat" ]]; then

    echo "** Running in cat mode for" "${@:$ITER}"

    ./bin/my_tar -cf fake.tar "${@:$ITER}"
    run_real_tar "${@:$ITER}" 

    echo "--"
    echo " "

    echo "** Comparing my_tar vs tar contents:"
    echo "real tar:"
    cat real.tar
    echo " "
    echo "my tar:"
    cat fake.tar
  fi
done
rm *tar
fi

