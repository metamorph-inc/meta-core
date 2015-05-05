#!/bin/bash
# Kevin Smyth @ ISIS 2-Jan-2014
# RPB @ SwRI 6-Nov-13

set -e
set -u
set -x
set -m

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

mpd &
sleep 1
function finish {
  kill %1
}
trap finish EXIT

if [ -f ~/cth-config.inc ]; then
  . ~/cth-config.inc
else
  export CTHPATH="/opt/CTH-10.3"
  export LD_LIBRARY_PATH=/opt/CTH-10.3/mpi/4.0.0.028/intel64/lib/
  export PATH=$PATH:/opt/CTH-10.3/bin64/
  export MPI_PROCESSORS=24
fi

for CTH_FILE in *.cth; do : ; done
mpiexec -n $MPI_PROCESSORS mpicth i="$CTH_FILE"

ffmpeg -r 10 -i mats-xy-plus-%04d.jpg xy-pos.mp4
ffmpeg -r 10 -i mats-xy-minus-%04d.jpg xy-neg.mp4
ffmpeg -r 10 -i mats-xz-plus-%04d.jpg xz-pos.mp4
ffmpeg -r 10 -i mats-xz-minus-%04d.jpg xz-neg.mp4

mv hscth tracerdata.csv

if [ ! "$KEEP_FILES" ]; then
 [ -f metamat ] && rm -v metamat
 shopt -s nullglob
 rm hct* *.jpg rsct* brct* spct* cthl* cthj*
fi
