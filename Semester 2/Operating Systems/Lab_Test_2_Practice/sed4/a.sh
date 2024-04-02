#!/bin/bash

sed -E 's/([0-9]{2,})/{\1}/g' file.txt > output.txt
