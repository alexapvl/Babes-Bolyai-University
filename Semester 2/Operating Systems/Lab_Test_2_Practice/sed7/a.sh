#!/bin/bash

sed -E 's/^(.)(.*)(.)$/\3\2\1/' file.txt > output.txt
