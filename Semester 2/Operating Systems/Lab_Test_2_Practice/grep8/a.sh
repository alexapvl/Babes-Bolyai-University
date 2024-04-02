#!/bin/bash

cat file.txt | grep -Eo ".[^\$]*$"
