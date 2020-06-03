rm *.zip
branch=`git rev-parse --abbrev-ref HEAD`
zip ${branch}.zip *.h *.cpp