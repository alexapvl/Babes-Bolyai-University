#! /bin/bash

cat file.txt | grep -E "^([13579]{2})+[^13579].*[^02468][02468]([02468]{2})*$"
