#!/bin/sh

# Poppulate a new directory from the template directory.

TARGET_DIR=$1
SOURCE_DIR="00_dir_template"

if [ -z "$TARGET_DIR" ]; then
  echo "!!! You must provide a target directory name"
  exit 1
fi

if [ -d "$TARGET_DIR" ]; then
  echo "!!! Directory name '${TARGET_DIR}' already exists, please resolve the conflict"
  #exit 2
fi

mkdir ${TARGET_DIR}
if [ $? != 0 ]; then
  echo "!!! target directory '${TARGET_DIR}' could not be created: $?"
  exit 3
fi

cp --recursive ${SOURCE_DIR}/* ${TARGET_DIR}
if [ $? != 0 ]; then
  echo "!!! copy from  '${SOURCE_DIR}/*' to '${TARGET_DIR}' failed: $?"
  exit 3
fi

cp ${SOURCE_DIR}/.gitignore ${TARGET_DIR}
if [ $? != 0 ]; then
  echo "!!! copy from  '${SOURCE_DIR}/.gitignore' to '${TARGET_DIR}' failed: $?"
  exit 3
fi
