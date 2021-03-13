#!/bin/sh
FILES=$(find src -regex ".*\.\(hpp\|cpp\|cc\|c\|h\)$")

if clang-format $FILES --output-replacements-xml | grep "<replacement " > replacements.xml ;then
  echo Some files require formatting
  echo Be sure to use this version of clang-format:
  clang-format --version
  exit 1;
else
  exit 0;
fi
