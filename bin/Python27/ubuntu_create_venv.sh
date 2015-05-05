#!/bin/bash

set -e

#http_proxy=kms1.isis.vanderbilt.edu:8888
#export http_proxy

# cache svn password
# svn cat https://svn.isis.vanderbilt.edu/META/trunk/bin/Python27/Lib/site-packages/PCC-py2.7.egg  > /dev/null

OPENMDAO_VERSION=${OPENMDAO_VERSION:-0.2.7}

# libfreetype6-dev:   building 'matplotlib.ft2font' extension
# libpng12-dev: building 'matplotlib._png' extension

REQUIRED_PACKAGES="python-virtualenv python-setuptools python-numpy python-scipy build-essential gfortran wget libfreetype6-dev libpng12-dev subversion"
# prereqs for compiling scipy: libblas-dev libatlas-base-dev (among others) (but we don't compile it)
dpkg -l $REQUIRED_PACKAGES >/dev/null || \
  sudo apt-get install -y $REQUIRED_PACKAGES
dpkg -l $REQUIRED_PACKAGES | tail -n+6 | grep -v ^ii && \
  sudo apt-get install -y $REQUIRED_PACKAGES


[ ! -f go-openmdao-$OPENMDAO_VERSION.py ] && wget -O go-openmdao-$OPENMDAO_VERSION.py http://openmdao.org/releases/$OPENMDAO_VERSION/go-openmdao.py
[ ! -d openmdao-$OPENMDAO_VERSION ] && python go-openmdao-$OPENMDAO_VERSION.py
# ./openmdao-0.2.7/bin/easy_install scipy
# ./openmdao-0.2.7/bin/easy_install pip
# ./openmdao-0.2.7/bin/python -m pip freeze
cat <<EOF > requirements.txt
elementtree==1.2.7-20070827-preview
matplotlib==1.1.0
mpmath==0.17
EOF
./openmdao-$OPENMDAO_VERSION/bin/pip install -r requirements.txt

./openmdao-$OPENMDAO_VERSION/bin/pip install 'svn+https://svn.isis.vanderbilt.edu/META/trunk/src/Python27Packages/PCC#egg=PCC'
./openmdao-$OPENMDAO_VERSION/bin/pip install 'svn+https://svn.isis.vanderbilt.edu/META/trunk/src/Python27Packages/py_modelica#egg=py_modelica'
