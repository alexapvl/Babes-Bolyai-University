#!/bin/bash

cat file.txt | grep -E "^[^a-zA-Z0-9]*$"
