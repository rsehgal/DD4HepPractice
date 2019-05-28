# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# DDCMS_FOUND          If the ROOT is found
# DDCMS_INCLUDE_DIR    PATH to the include directory
# DDCMS_LIBRARIES      Most common libraries
# DDCMS_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable DDCMSROOT exists
set(DDCMSROOT $ENV{DDCMS_DIR})

find_library(DDCMS_LIBRARIES libDDCMS.so PATHS ${DDCMSROOT}/lib)
if (DDCMS_LIBRARIES) 
   set(DDCMS_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" DDCMSROOT  ${DDCMS_LIBRARIES})
   set(DDCMS_INCLUDE_DIR ${DDCMSROOT}/include)
   set(DDCMS_LIBRARY_DIR ${DDCMSROOT}/lib)
   #collecting all the DDCMS libraries
   file(GLOB DDCMS_LIBRARIES ${DDCMS_LIBRARIES} ${DDCMS_LIBRARY_DIR}/*.so)
   message(STATUS "Found DDCMS library in ${DDCMS_LIBRARY_DIR}")		
else()
   message(STATUS "DDCMS library not found; try to set a DDCMSROOT environment variable to the base installation path or add -DDDCMSROOT= to the cmake command")	
endif()

