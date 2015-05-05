#!/bin/bash

set -x
set -u

KEEP_FILES=
for i in "$@"
do
case $i in
    --keep-files)
    KEEP_FILES=true
    ;;
    *)
    ;;
esac
done

if [ -f ~/lsdyna-config.inc ]; then
  . ~/lsdyna-config.inc
else
  # set LS-DYNA environment variables
  export LSTC_LICENSE=${LSTC_LICENSE-network}
  export LSTC_LICENSE_SERVER=${LSTC_LICENSE_SERVER-10.2.204.24}
  export LSTC_LICENSE_SERVER_PORT=${LSTC_LICENSE_SERVER_PORT-31010}
  export LSTC_NCPU=${LSTC_NCPU-8}
  export LSTC_MEMORY=${LSTC_MEMORY-auto}

  # set LS-PrePost environment variables
  DN=${DN-/usr/local/lsprepost4.1_centos6}
  export LD_LIBRARY_PATH=$DN/lib:${LD_LIBRARY_PATH-}
fi

# unzipping results directory
Dyna_Result_Dir=BlastConfig-DYNA-deck
if [ -d $Dyna_Result_Dir ]; then
    rm -rf $Dyna_Result_Dir
fi

if [ ! -f BlastConfig-DYNA-deck.zip ]; then
    echo "BlastConfig-DYNA-deck does not exist!" >> _FAILED.txt
    exit 2
fi

mkdir $Dyna_Result_Dir
pushd $Dyna_Result_Dir
unzip ../BlastConfig-DYNA-deck.zip || exit $?


# run LS-DYNA
dyna_cmd=`which ls-dyna`
if [ "$dyna_cmd" == "" ]; then
    echo "ls-dyna not installed on worker" >> ../_FAILED.txt
    exit 2
fi

$dyna_cmd NCPU=${LSTC_NCPU} i=run.k
if [ ! -f messag ]; then
    echo "runLSDyna.sh, LS_DYNA failed to produce messag file during run, run must have failed" >> ../_FAILED.txt
    exit 2
fi

MESSAG_ERROR=`grep -i "e r r o r" messag`
if [ "$MESSAG_ERROR" != "" ]; then
    echo "runLSDyna.sh, LS_DYNA error see BlastConfig-DYNA-deck/messag file for details" >> ../_FAILED.txt
    echo $MESSAG_ERROR >> ../_FAILED.txt
    exit 2
fi


xvfb-run -n 1 -s "-once -screen 0 1280x1024x24" $DN/lsprepost c=postprocess.cfile

if [ -f lspost.msg ]; then
   LSPOST_ERROR=`grep -i "error" lspost.msg`
   if [ "$LSPOST_ERROR" != "" ] ; then
	echo "runLSDyna.sh, LS-PrePost error see BlastConfig-DYNA-deck/lspost.msg file for details" >> ../_FAILED.txt
	exit 2
   fi
fi

if [ ! "$KEEP_FILES" ]; then
 rm *.k *.cfile
fi
zip BlastConfig-DYNA-results.zip ./*
mv BlastConfig-DYNA-results.zip ../
popd
rm -rf BlastConfig-DYNA-deck                                       
