#To put all the executables in build directory
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

enable_language(CXX)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} _build_type)

# Setting the C++ compiler to use
set("${CMAKE_CXX_COMPILER_ID}" TRUE CACHE STRING "C++ Compiler")
if (NOT GNU AND NOT Clang AND NOT Intel)
  message(WARNING "Unsupported compiler. Build will likely fail.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(MacroUtilities)
################################################################################

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIR})

find_package(DD4hep REQUIRED)
if(DD4hep_FOUND)
	set(DD4hep_DIR $ENV{DD4hep_DIR})
	include_directories(${DD4hep_INCLUDE_DIR})
	message(STATUS "Following DD4hep Libraries linked : ${DD4hep_LIBRARIES}")
endif(DD4hep_FOUND)

#add_executable(test_unit ${CMAKE_CURRENT_SOURCE_DIR}/src/test_units.cc)
#target_link_libraries(test_unit ${DD4hep_LIBRARIES} ${ROOT_LIBRARIES})

#Copying compact xml files
file(GLOB COMPACT_FILES ${PROJECT_SOURCE_DIR}/compact/*)
file(COPY ${COMPACT_FILES} DESTINATION ${PROJECT_BINARY_DIR})

#COLLECTING ALL TEST FILES
macro(collect_tests TESTS)
  foreach(TEST ${TESTS})
    file(GLOB tests ${tests}
	    ${TEST}/*.cpp
	    ${TEST}/*.cc )
  endforeach()
endmacro()

macro(build_executables EXECUTABLES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    add_executable(${TARGET_NAME} ${EXECUTABLE})
    target_link_libraries(${TARGET_NAME} ${DD4hep_LIBRARIES} ${ROOT_LIBRARIES})
  endforeach()
endmacro()

macro(link_executables EXECUTABLES TARGET_LIBRARIES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    #add_executable(${TARGET_NAME} ${EXECUTABLE})
    target_link_libraries(${TARGET_NAME} ${TARGET_LIBRARIES})
  endforeach()
endmacro()


