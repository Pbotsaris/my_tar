#!/bin/bash
<<<<<<< HEAD
<<<<<<< HEAD
=======
rm *.tar
if [[ "$OSTYPE" == "darwin"* ]]; then
  gtar -cf real.tar test_files/f.txt
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  tar -cf real.tar test_files/f.txt
fi
./bin/my_tar -cf $1 fake.tar test_files/f.txt
echo "real"
cat real.tar
echo "fake"
cat fake.tar
>>>>>>> 9c69f62e56e6ccffd16c4effe5533e806b1ed1fb

=======
>>>>>>> 9a66417fa5a59ec156dada4c2c52e6fa32fce87a
run_tar(){
  if [[ "$OSTYPE" == "darwin"* ]]; then
    gtar -cf real.tar $1
  elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    tar -cf real.tar $1 
  fi
}

if [ -z $1 ]; then
  echo "please enter a filename"
else
  rm *.tar

  if [[ $1 == "-d" ]]; then
    ./bin/my_tar -cf $1 fake.tar $2 
    run_tar $2
  else
    ./bin/my_tar -cf fake.tar $1 
    run_tar $1
  fi

  echo "real"
  cat real.tar
  echo " "
  echo "fake"
  cat fake.tar

fi
