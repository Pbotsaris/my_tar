#!bin/bash
rm *.tar
if [[ "$OSTYPE" == "darwin"* ]]; then
  gtar -cf real.tar test_files/f.txt
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  tar -cf real.tar test_files/f.txt
fi
./bin/my_tar -s $1 fake.tar test_files/f.txt
cat real.tar
cat fake.tar



