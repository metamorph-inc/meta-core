#!/bin/sh

echo "Checking prerequisites:"

echo -en "\tScript begin run on Windows OS ... "
if uname | grep -i linux > /dev/null 2>&1; then
  echo "no"
  echo "This script should only be run on a Windows OS."
  exit 1
fi
echo "yes"


echo -en "\tUDM is installed ... "
if [ -z "$UDM_PATH" ]; then
  echo "no"
  echo "The UDM_PATH variable is not set.  This indicates that UDM is not installed."
  echo "Please install UDM before running this script."
  exit 2
fi
echo "yes"

echo -en "\tGReAT is installed ... "
if [ -z "$GREAT_PATH" ]; then
  echo "no"
  echo "The GREAT_PATH variable is not set.  This indicates that GReAT is not installed."
  echo "Please install GReAT before running this script."
  exit 3
fi
echo "yes"

echo -en "\tVCP_PATH environment variable is set ... "
if [ -z "$VCP_PATH" ]; then
  echo "no"
  echo "The VCP_PATH variable is not set.  Please set the VCP_PATH to the trunk directory of the HCDDES project like this:"
  echo "export VCP_PATH=\"<directory>\""
  echo "where <directory> is the trunk directory of the HCDDES project.  Example:"
  echo "export VCP_PATH=\"/home/hnine/Projects/HCDDES/trunk\""
  exit 4
fi
echo "yes"

echo "Prerequisites met"
echo


cd "$(cygpath "$VCP_PATH")"


function gen_code() {

  paradigm_name=$1
  xme_path=$2

  meta_paradigm="MetaGME"
  uml_name="${paradigm_name}_uml.mga"
  if grep -q 'metaname="UML"' "$xme_path" > /dev/null 2>&1; then
    meta_paradigm="UML"
    uml_name="${paradigm_name}.mga"
  fi
  
  echo "Generating $paradigm_name source:"
 
  echo -en "\tCreating directories ... "
  mkdir -p generated/$paradigm_name/models
  mkdir -p generated/$paradigm_name/generated
  echo "done"

  echo -en "\tConverting $paradigm_name xme to mga ... "
  if ! common-scripts/gmepy.exe xme2mga  "$xme_path" generated/$paradigm_name/models/$paradigm_name.mga > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 1
  fi
  echo "done"; rm temp$$

  pushd generated/$paradigm_name/models >/dev/null 2>&1

  if [ "$meta_paradigm" == "MetaGME" ]; then
    echo -en "\tTranslating $paradigm_name to UML ... "
    if ! ../../../common-scripts/gmepy.exe meta2uml "$paradigm_name.mga" "$uml_name" > temp$$ 2>&1; then
      echo -e"failed\nError is:"; cat temp$$; rm temp$$
      return 2
    fi
    echo "done"; rm temp$$
  fi

  echo -en "\tTranslating UML to UDM ... "
  if ! ../../../common-scripts/gmepy.exe mga2udmxml "$uml_name" > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 3
  fi
  echo "done"; rm temp$$

  popd > /dev/null 2>&1
  pushd generated/$paradigm_name/generated > /dev/null 2>&1

  echo -en "\tGenerating $paradigm_name C++ API ... "
  if ! "${UDM_PATH}/bin/Udm.exe" -g ../models/${paradigm_name}_udm.xml > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 4
  fi
  echo "done"; rm temp$$

  popd > /dev/null 2>&1 

  echo "$paradigm_name source generation complete."

  return 0
}

function gen_great_code() {


  paradigm_name=$1
  project_name=$2
  transform_name=$3
  xme_path=$4

  subdir_name="ESMoL"
  relparentdir=".."
  if [ "$project_name" == "ctestgen" ]; then
    subdir_name=""
    relparentdir=""
  fi

  echo "Generating $project_name source:"
 
  echo -en "\tCreating directories ... "
  mkdir -p generated/$project_name/models/ESMoL
  mkdir -p generated/$project_name/generated/ESMoL
  echo "done"

  echo -en "\tConverting $paradigm_name xme to mga ... "
  if ! common-scripts/gmepy.exe xme2mga  "$xme_path" "generated/$project_name/models/$subdir_name/$transform_name.mga" > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 1
  fi
  echo "done"; rm temp$$

  echo -en "\tRunning GReAT Master Interpreter ... "
  if ! common-scripts/gmepy.exe RunGreatMasterInt "generated/$project_name/models/$subdir_name/$transform_name.mga" > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 2
  fi
  echo "done"; rm temp$$

  echo -en "\tRunning Code Generateor ... "
  if ! "${GREAT_PATH}/bin/CG.exe" "../../generated/$subdir_name/Gen$transform_name" "generated/$project_name/models/$subdir_name/Config.mga" > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 3
  fi
  echo "done"; rm temp$$

  pushd "generated/$project_name/generated/$subdir_name" > /dev/null 2>&1

  echo -en "\tGenerating $paradigm_name transform C++ API ... "
  if ! "${UDM_PATH}/bin/Udm.exe" -g "../$relparentdir/models/$subdir_name/Udm/$transform_name.udm" > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 4
  fi
  echo "done"; rm temp$$

  popd > /dev/null 2>&1 

  echo -en "\tCopying GReAT-generated header files ... "
  if ! cp "${USERPROFILE}/Local Settings/Temp/GReAT/"$transform_name*.h GReAT/include > temp$$ 2>&1; then
    echo -e "failed\nError is:"; cat temp$$; rm temp$$
    return 5
  fi
  echo "done"; rm temp$$

  echo "$project_name source generation complete."

  return 0
}


#
# RTT PROCESSING
#
mkdir -p GReAT/include
mkdir -p GReAT/src

echo "To build the zip file, a source file from GReAT repository at ISIS must be downloaded."
echo "Please enter username and password for \"https://svn.isis.vanderbilt.edu/MoBIES/GReAT/trunk/Libraries/RTT\":"
echo -n "Username: "
read username
echo -n "Password (not echoed): "
stty -echo
read password
stty echo
echo

echo -n "Downloading source file ... "
if ! wget "https://svn.isis.vanderbilt.edu/MoBIES/GReAT/trunk/Libraries/RTT/RTTGenerator.cpp" --no-check-certificate --user="$username" --password="$password" -O GReAT/src/RTTGenerator.cpp > temp$$ 2>&1; then
  unset username password; echo -e "failed\nError is:"; cat temp$$; rm temp$$
  exit 1
fi
unset username password; echo "done"; rm temp$$

gen_code RTT "$GREAT_PATH/Meta/RTT_meta.xme"
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 4 ))
fi

echo -en "Copying RTT files into GReAT directory ... "
cp generated/$paradigm_name/generated/*.h* GReAT/include
cp generated/$paradigm_name/generated/*.cpp GReAT/src
echo "done"
echo



#
# ESMoL PROCESSING
#
gen_code ESMoL src/models/ESMoL/ESMoL.xme
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 8 ))
fi
unset error_code
echo



#
# SFC PROCESSING
#
gen_code SFC src/models/SFC/SFC.xme
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 12 ))
fi
unset error_code
echo



#
# ESM2SLC PROCESSING
#
gen_great_code ESM2SLC ESM2SLC ESM2SLC src/models/ESM2SLC/ESM2SLC.xme
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 16 ))
fi
unset error_code
echo



#
# SF_CodeGen PROCESSING
#
gen_great_code ESM2SFC SF_CodeGen ESM2SFC src/SF_CodeGen/models/ESM2SFC.xme
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 21 ))
fi
unset error_code
echo



#
# ctestgen PROCESSING
#
gen_great_code ctestgen ctestgen ctest src/ctestgen/ctest.xme
error_code=$?
if [ "$error_code" -ne 0 ]; then
  exit $(( $error_code + 26 ))
fi
unset error_code
echo



#
# CREATE ZIP
#
echo -n "zipping files into Linux/linux.zip in \"$VCP_PATH\" ... "
rm Linux/linux.zip
zip -qq -r Linux/linux.zip GReAT/include GReAT/src/*.cpp generated/ESM2SLC/generated/ generated/SF_CodeGen/generated generated/ESMoL/generated generated/SFC/generated generated/ctestgen/generated
echo "done."

