rm *.tar
  gtar -cf real.tar test_files/f.txt
  ./bin/my_tar -s $1 fake.tar test_files/f.txt
  cat real.tar
  cat fake.tar



