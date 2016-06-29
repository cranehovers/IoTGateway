#!/bin/sh

WK_DIR=`pwd`

echo ${WK_DIR}

cd ${WK_DIR}/build
cmake ../
make
cd -

