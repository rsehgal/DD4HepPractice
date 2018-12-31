# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# DD4hep_FOUND          If the ROOT is found
# DD4hep_INCLUDE_DIR    PATH to the include directory
# DD4hep_LIBRARIES      Most common libraries
# DD4hep_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable DD4hepROOT exists
set(DD4hepROOT $ENV{DD4hep_DIR})

find_library(DD4hep_LIBRARIES libDDCore.so PATHS ${DD4hepROOT}/lib)
if (DD4hep_LIBRARIES) 
   set(DD4hep_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" DD4hepROOT  ${DD4hep_LIBRARIES})
   set(DD4hep_INCLUDE_DIR ${DD4hepROOT}/include)
   set(DD4hep_LIBRARY_DIR ${DD4hepROOT}/lib)
   message(STATUS "Found DD4hep library in ${DD4hep_LIBRARIES}")		
else()
   message(STATUS "DD4hep library not found; try to set a DD4hepROOT environment variable to the base installation path or add -DDD4hepROOT= to the cmake command")	
endif()

