#!/bin/bash
run_tar(){
  if [[ "$OSTYPE" == "darwin"* ]]; then
    gtar -cf real.tar $1 $2
  elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    tar -cf real.tar $1 $2
  fi
}

if [ -z $1 ]; then
  echo "please enter a filename"
else
  rm *.tar

  if [[ $1 == "-d" ]]; then
    ./bin/my_tar -cf $1 fake.tar $2 $3 
    run_tar $2 $3
  else
    ./bin/my_tar -cf fake.tar $1 $2
    run_tar $1 $2
  fi

  echo "real"
  cat real.tar
  echo " "
  echo "fake"
  cat fake.tar

fi
