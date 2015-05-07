#!/bin/bash
echo ------------------------------------------------------
echo Running adtest1... full schedule on a single processor
SchedToolD -f adtest1.scs -o adtest1.tmprslt -b min
echo Comparing results
diff adtest1.rslt adtest1.tmprslt > adtest1.out
if [[ -s adtest1.out ]] ; then
echo ------------------------------------------------------
echo Differences:
cat adtest1.out
echo ------------------------------------------------------
echo Remember to delete adtest1.tmprslt and adtest1.out
echo  when you are finished debugging.
else
echo No differences found -- test successful.
rm -f adtest1.tmprslt adtest1.out
fi ;
echo ------------------------------------------------------

