#!/bin/sh

if [ -z "$VCP_PATH" ]; then
  echo "The VCP_PATH variable is not set.  Please set the VCP_PATH to the trunk directory of the HCDDES project like this:"
  echo "export VCP_PATH=\"<directory>\""
  echo "where <directory> is the trunk directory of the HCDDES project.  Example:"
  echo "export VCP_PATH=\"/home/hnine/Projects/HCDDES/trunk\""
  exit 1
fi

cd "$VCP_PATH"

if [ ! -e Linux/linux.zip ]; then
  echo "Could not find the \"linux.zip\" file in the \$VCP_PATH/Linux directory \"$VCP_PATH/Linux\"."
  echo "Is the VCP_PATH variable set correctly?  Please set it to the trunk directory of the HCDDES project like this:"
  echo "export VCP_PATH=\"<directory>\""
  echo "where <directory> is the trunk directory of the HCDDES project.  Example:"
  echo "export VCP_PATH=\"/home/hnine/Projects/HCDDES/trunk\""
  exit 2
fi

rm -rf generated/*/generated
rm -rf GReAT/src/*.cpp
rm -rf GReAT/include

echo -n "Unzipping unix.zip in \"$VCP_PATH\" ... "
unzip -qq -o Linux/linux.zip
echo "done."

