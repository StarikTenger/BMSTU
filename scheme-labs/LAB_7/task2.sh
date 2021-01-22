#!/bin/bash
C_STRINGS="$((find $1 -name '*.cpp' -print0 | xargs -0 cat ) | cat | sed '/^\s*$/d' | wc -l)"
H_STINGS="$((find $1 -name '*.h' -print0 | xargs -0 cat ) | cat | sed '/^\s*$/d' | wc -l)"
SUM=$(($C_STRINGS+$H_STINGS))
echo "${SUM}"