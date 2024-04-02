#!/bin/bash

sed 's/^..//' file.txt | sed 's/...$//' > output.txt
