# Copyright: (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ugo Pattacini <ugo.pattacini@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 3.5)

# set the project name
project(TestTutorialCartesianInterface)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
set(CMAKE_SHARED_MODULE_PREFIX "")

# add the required cmake packages
find_package(RobotTestingFramework 2.0.0 COMPONENTS DLL)
find_package(YARP 3.5.1 REQUIRED COMPONENTS os sig dev math robottestingframework)

# add include directories
include_directories(${CMAKE_SOURCE_DIR}
                    ${RobotTestingFramework_INCLUDE_DIRS})

# add the source codes to build the plugin library
add_library(${PROJECT_NAME} MODULE test.cpp)

# import math symbols from standard cmath
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

# add required libraries
target_link_libraries(${PROJECT_NAME} ${RobotTestingFramework_LIBRARIES} ${YARP_LIBRARIES})
