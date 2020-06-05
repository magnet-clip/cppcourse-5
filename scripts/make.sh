if [ -e last_build ]; then
  source last_build
  make
else
  echo "Run CMAKE first"
fi