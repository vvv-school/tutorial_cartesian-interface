#!/bin/bash

# Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ugo Pattacini <ugo.pattacini@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v3.0.

rm build -rf
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../../
make install
if [ $? -ne 0 ]; then
   exit 1
fi
cd ../

rm build-test -rf
mkdir build-test && cd build-test
cmake -DCMAKE_BUILD_TYPE=Release ../
make
if [ $? -ne 0 ]; then
   exit 2
fi
cd ../

# to let yarpmanager access the fixture
export YARP_DATA_DIRS=${YARP_DATA_DIRS}:$(pwd)

# to make the test library retrievable
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/build-test/plugins

yarp where > /dev/null 2>&1
if [ $? -eq 0 ]; then
   kill_yarp="no"
else
   kill_yarp="yes"
   yarpserver --write &
   sleep 1 
fi

testrunner --verbose --suit test.xml > output.txt

if [ "$kill_yarp" == "yes" ]; then
   killall yarpserver
fi

cd build
make uninstall && cd ../

cat output.txt

ok=$(grep -i "Number of passed test cases" output.txt | sed 's/[^0-9]*//g')
if [ $ok -eq 0 ]; then
   echo "xxxxx Test FAILED xxxxx"
   exit 3
else
   echo "===== Test PASSED ====="
   exit 0
fi
