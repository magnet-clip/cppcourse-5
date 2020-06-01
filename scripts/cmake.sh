if [ $1 == "clang" ]; then
  export CXX=/usr/bin/clang++
  export USE_SANITIZER=NO
elif  [ $1 == "sanitize" ]; then
  export CXX=/usr/bin/clang++
  export USE_SANITIZER=YES
else
  export USE_SANITIZER=NO
fi
cmake .. && make