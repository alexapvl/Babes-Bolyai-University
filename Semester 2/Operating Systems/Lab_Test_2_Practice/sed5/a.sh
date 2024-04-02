#!/bin/bash

sed -E 's/([^ ]+) ([^ ]+)(.*)/\1\3/g' file.txt > output.txt
