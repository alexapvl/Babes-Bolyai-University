#!/bin/bash

sed -E 's/[AEIOU]{3,}//g' file.txt > output.txt
