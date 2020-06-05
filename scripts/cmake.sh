case "$1" in
  "release") 
    echo "Release Mode"
    if [[ -n "$2" && $2 == "clang" ]]; then
      echo "Using Clang"
      export CXX=/usr/bin/clang++
    else
      echo "Using Gcc"
    fi
    export MODE=RELEASE
    ;; 
  "test")
    echo "Test Mode"
    if [[ -n "$2" && $2 == "clang" ]]; then
      echo "Using Clang"
      export CXX=/usr/bin/clang++
    else 
      echo "Using Gcc"
    fi
    export MODE=TEST
    ;;
  "debug")
    echo "Debug Mode"
    echo "Using Clang"
    export CXX=/usr/bin/clang++
    export MODE=DEBUG
    ;;
  *)
    echo "Unknown parameter $1"
    exit 1
    ;;
esac

cmake .. 

echo "MODE=$MODE" >> last_build
if [ -n "$CXX" ]; 
  echo "CXX=$CXX" >> last_build
fi

make