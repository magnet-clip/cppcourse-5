if [ $1 == "release" ]; then
  echo "Release Mode"
  if [[ -n "$2" && $2 == "clang" ]]; then
  echo "Using Clang"
    export CXX=/usr/bin/clang++
  else
    echo "Using Gcc"
  fi
  export DEBUG=NO
elif  [ $1 == "test" ]; then
  echo "Test Mode"
  if [[ -n "$2" && $2 == "clang" ]]; then
    echo "Using Clang"
    export CXX=/usr/bin/clang++
  else 
    echo "Using Gcc"
  fi
  export DEBUG=NO
elif  [ $1 == "debug" ]; then
  echo "Debug Mode"
  echo "Using Clang"
  export CXX=/usr/bin/clang++
  export DEBUG=YES
else
  echo "Unknown parameter $1"
  exit 1
fi

cmake .. && make