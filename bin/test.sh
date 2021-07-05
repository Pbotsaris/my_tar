#!/bin/bash

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
    ./bin/my_tar -s $1 fake.tar $2 
    run_tar $2
  else
    ./bin/my_tar -s fake.tar $1 
    run_tar $1
  fi

  echo "real"
  cat real.tar
  echo "fake"
  cat fake.tar

fi
