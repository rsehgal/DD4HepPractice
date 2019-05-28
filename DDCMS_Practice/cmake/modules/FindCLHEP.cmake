# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# CLHEP_FOUND          If the ROOT is found
# CLHEP_INCLUDE_DIR    PATH to the include directory
# CLHEP_LIBRARIES      Most common libraries
# CLHEP_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable CLHEPROOT exists
set(CLHEPROOT $ENV{CLHEP_DIR})

find_library(CLHEP_LIBRARIES libCLHEP.so PATHS ${CLHEPROOT}/lib)
if (CLHEP_LIBRARIES) 
   set(CLHEP_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" CLHEPROOT  ${CLHEP_LIBRARIES})
   set(CLHEP_INCLUDE_DIR ${CLHEPROOT}/include)
   set(CLHEP_LIBRARY_DIR ${CLHEPROOT}/lib)
   #collecting all the CLHEP libraries
   file(GLOB CLHEP_LIBRARIES ${CLHEP_LIBRARIES} ${CLHEP_LIBRARY_DIR}/*.so)
   message(STATUS "Found CLHEP library in ${CLHEP_LIBRARY_DIR}")		
else()
   message(STATUS "CLHEP library not found; try to set a CLHEPROOT environment variable to the base installation path or add -DCLHEPROOT= to the cmake command")	
endif()

