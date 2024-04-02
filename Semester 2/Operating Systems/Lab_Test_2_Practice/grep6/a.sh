#!/bin/bash

cat file.txt | grep -Ec "^([A-Z])+$"
