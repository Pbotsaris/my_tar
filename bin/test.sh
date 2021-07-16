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
    echo "----------------------------------"
    echo "----------------------------------"
    echo "** Testing " "$file"
    echo "----------------------------------"
    echo "----------------------------------"
    echo " "
    echo "Archiving:"
   ./bin/my_tar -cf t.tar "$file"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Appending same file with -uf not modified returns an error:"
   ./bin/my_tar -uf t.tar "$file"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Appending another file with -rf:"
    echo "another file to append" > test_files/append.txt
   ./bin/my_tar -rf t.tar test_files/append.txt
    rm test_files/append.txt
    echo "-"

    echo "List all files in archive:"
   ./bin/my_tar -tf t.tar 
    echo "-"

    echo "Creating output folder for extraction:"
    mkdir output
    mv t.tar output/t.tar
    echo " "

    echo "CDing to output and xxtracting files:"
    cd output
    ../bin/my_tar -xf t.tar
    echo " "

    echo "CDing t parrent and listing output folder:"
    cd ..
    ls -la output/test_files
    echo "-"

    echo "Removing output folder."
    rm -rf output/test_files
  done
  echo "SUCCESS!"
else

for arg in "$@"; do

  ITER=$(expr $ITER + 1)
  if [[ $arg == "-d" ]]; then

    ./bin/my_tar "$arg" "${@:$ITER}"

  elif [[ $arg == "--append" ]]; then

    echo "Test appending with -uf. Create tar with" "$2"
   ./bin/my_tar -cf t.tar "$2"
    cat t.tar
    echo "Modify" "$2"
    echo "--> this file has been modified" > "$2"
    echo "Appending..."
   ./bin/my_tar -uf t.tar "$2"
   echo "After:"
    cat t.tar

   ./bin/my_tar -tf t.tar "$2"

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

