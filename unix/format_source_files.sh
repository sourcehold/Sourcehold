#!/bin/sh

clang-format $(find src -regex ".*\.\(hpp\|cpp\|cc\|c\|h\)$") --style=file -i
