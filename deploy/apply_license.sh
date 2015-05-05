#!/bin/bash

LICENSE=../license.txt

set -e
set -u

[ -f "$LICENSE" ] || ( echo File does not exist\: $LICENSE; false)

find \( -iname \*.cpp -or -iname \*.c -or -iname \*.h -or -iname \*.hpp -or -iname \*.cs \) -print |
while read -r file; do
echo $file
( echo /\*; cat "$LICENSE"; echo \*/; cat "$file") > tmp
cat tmp > "$file"
done

rm tmp
