#!/bin/bash

# Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ugo Pattacini <ugo.pattacini@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v3.0.

rm build -rf
mkdir build && cd build
cmake ../../
make install && cd ../

rm build-test -rf
mkdir build-test && cd build-test
cmake -DCMAKE_BUILD_TYPE=Release ../
make && cd ../

# to let yarpmanager access the fixture
if [ -z "$YARP_DATA_DIRS" ]; then
	export YARP_DATA_DIRS=$(pwd)
else
	export YARP_DATA_DIRS=${YARP_DATA_DIRS}:$(pwd)
fi

# to make the test library retrievable
if [ -z "$LD_LIBRARY_PATH" ]; then
	export LD_LIBRARY_PATH=$(pwd)/build-test/plugins
else
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/build-test/plugins
fi

if ! yarp where
then
   kill_yarp="yes"
   yarpserver --write &
   sleep 1
else
   kill_yarp="no"
fi

testrunner --verbose --suit test.xml

cd build
make uninstall && cd ../

if [ "$kill_yarp" == "yes" ]; then
   killall yarpserver
fi