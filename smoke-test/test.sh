#!/bin/bash

# Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ugo Pattacini <ugo.pattacini@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v3.0.

echo "Installing the tutorial"
rm build -rf
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../../ > /dev/null 2>&1
make install > /dev/null 2>&1
cd ../

echo "Compiling the test"
rm build-test -rf
mkdir build-test && cd build-test
cmake -DCMAKE_BUILD_TYPE=Release ../ > /dev/null 2>&1
make > /dev/null 2>&1
cd ../

# to let yarpmanager access the fixture
export YARP_DATA_DIRS=${YARP_DATA_DIRS}:$(pwd)

# to make the test library retrievable
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/build-test/plugins

if ! yarp where > /dev/null 2>&1
then
   kill_yarp="yes"
   yarpserver --write > /dev/null 2>&1 &
   sleep 1
else
   kill_yarp="no"
fi

testrunner --verbose --suit test.xml
if [ $? -eq 0 ]; then 
   echo "TEST PASSED!"
else
   echo "TEST FAILED!"
fi

if [ "$kill_yarp" == "yes" ]; then
   killall yarpserver > /dev/null 2>&1
fi

echo "Uninstalling the tutorial"
cd build
make uninstall > /dev/null 2>&1
cd ../
