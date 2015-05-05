#!/bin/bash

set -e

OPENMDAO_VERSION=${OPENMDAO_VERSION:-0.2.7}

REQUIRED_PACKAGES="python-virtualenv python-setuptools python-numpy python-scipy build-essential gfortran wget libfreetype6-dev libpng12-dev subversion"
# prereqs for compiling scipy: libblas-dev libatlas-base-dev (among others) (but we don't compile it)
dpkg -l $REQUIRED_PACKAGES >/dev/null || \
  sudo apt-get install -y $REQUIRED_PACKAGES
dpkg -l $REQUIRED_PACKAGES | tail -n+6 | grep -v ^ii && \
  sudo apt-get install -y $REQUIRED_PACKAGES

[ ! -f go-openmdao-$OPENMDAO_VERSION.py ] && wget -O go-openmdao-$OPENMDAO_VERSION.py http://openmdao.org/releases/$OPENMDAO_VERSION/go-openmdao.py
[ ! -d openmdao-$OPENMDAO_VERSION ] && python go-openmdao-$OPENMDAO_VERSION.py

cat <<EOF > requirements.txt
elementtree==1.2.7-20070827-preview
matplotlib==1.1.0
mpmath==0.17
EOF
./openmdao-$OPENMDAO_VERSION/bin/pip install -r requirements.txt

pushd ./py_modelica
~/openmdao-$OPENMDAO_VERSION/bin/python setup.py install
popd
pushd ./PCC
~/openmdao-$OPENMDAO_VERSION/bin/python setup.py install
popd
