cmake_minimum_required(VERSION 3.4)
project(DD4hepPractice C CXX)

include(../common.cmake)

collect_tests(${CMAKE_CURRENT_SOURCE_DIR}/src/)
build_executables("${tests}")
