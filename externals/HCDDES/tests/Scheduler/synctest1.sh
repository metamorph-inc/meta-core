#!/bin/bash
echo ------------------------------------------------------
echo Running synctest1... bus synchronization
SchedTool -f synctest1.scs -o synctest1.tmprslt -b med
echo Comparing results
diff synctest1.rslt synctest1.tmprslt > synctest1.out
if [[ -s synctest1.out ]] ; then
echo ------------------------------------------------------
echo Differences:
cat synctest1.out
echo ------------------------------------------------------
echo Remember to delete synctest1.tmprslt and synctest1.out
echo  when you are finished debugging.
else
echo No differences found -- test successful.
rm -f synctest1.tmprslt synctest1.out
fi ;
echo ------------------------------------------------------

