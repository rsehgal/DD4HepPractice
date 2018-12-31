cmake_minimum_required(VERSION 3.4)
project(DDEve C CXX)

include(../common.cmake)

collect_tests(${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "TESTS : ${tests}")
build_executables("${tests}")