#=================================================================
# Include guard ==================================================
#=================================================================
if (DEFINED BANG_CHECK_INPUT_PARAMS_BUILT)
    return()
endif()
set(BANG_CHECK_INPUT_PARAMS_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

if ( NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    AND
     NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Release")  AND
     NOT("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
   )

    message(FATAL_ERROR "Please specify the build type -DCMAKE_BUILD_TYPE=(Debug|Release|RelWithDebInfo)")

endif()
