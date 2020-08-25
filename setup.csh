#!/bin/tcsh
set sourced=($_)
if ("$sourced" != "") then
  set RELDIR="`dirname $sourced[2]`"
else if ("$0" != "tcsh") then
  set RELDIR="`dirname $0`"
endif

set DIR=`cd $RELDIR && pwd`

if (! $?LD_LIBRARY_PATH) then
  setenv LD_LIBRARY_PATH ${DIR}/build
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${DIR}/build
endif

if (! $?JAVA_OPTS) then
  setenv JAVA_OPTS "-Djava.library.path=${DIR}/build"
else
  setenv JAVA_OPTS "${JAVA_OPTS} -Djava.library.path=${DIR}/build"
endif

if (! $?JYPATH) then
  setenv JYPATH "${DIR}/target/*"
else
  setenv JYPATH "${JYPATH}:${DIR}/target/*"
endif


