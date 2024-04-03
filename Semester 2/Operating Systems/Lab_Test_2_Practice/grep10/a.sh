#!/bin/bash

grep --color=auto -E "\^([^aeiouAEIOU]*[aeiouAEIOU][^aeiouAEIOU]*){0,5}\^" file.txt
