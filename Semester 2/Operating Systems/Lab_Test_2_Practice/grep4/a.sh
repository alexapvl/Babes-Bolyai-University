#!/bin/bash

cat file.txt | grep -E "^([^AEIOU]*[AEIOU]?){0,3}[^AEIOU]*$" | grep -Ev "[13579]"
