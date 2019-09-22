#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${DIR}/build
export JAVA_OPTS="-Djava.library.path=${DIR}/build"
export JYPATH="${DIR}/target/*"
