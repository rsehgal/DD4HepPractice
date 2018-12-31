cmake_minimum_required(VERSION 3.4)
project(DDG4 C CXX)

include(../common.cmake)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

collect_tests(${CMAKE_CURRENT_SOURCE_DIR}/test)
#message(STATUS "TESTS : ${tests}")
build_executables("${tests}")
link_executables("${tests}" "${Geant4_LIBRARIES}")
