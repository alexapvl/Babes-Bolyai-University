#!/bin/bash

sed -E 's/([A-Z])/\1\1\1/g' file.txt > output.txt
