#! /bin/bash

cat file.txt | grep -E "([AEIOU].*){3,}" | grep -E -v "[13579]"
