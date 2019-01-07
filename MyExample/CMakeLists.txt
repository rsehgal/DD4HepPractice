cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
include(../common.cmake)
include ( ${DD4hep_DIR}/cmake/DD4hep.cmake )

#-----------------------------------------------------------------------------------
dd4hep_configure_output()
dd4hep_package (SimpleHollowTube MAJOR 0 MINOR 0 PATCH 1
  USES         [ROOT   REQUIRED COMPONENTS Geom GenVector]
               [DD4hep REQUIRED COMPONENTS DDCore DDRec]
  OPTIONAL     XERCESC
  INCLUDE_DIRS include )
#-----------------------------------------------------------------------------------
dd4hep_add_plugin(SimpleHollowTube SOURCES src/*.cpp)
