#!/bin/bash

cat file.txt | grep -En "^[0-9]{2}.*[0-9]{2}$"
