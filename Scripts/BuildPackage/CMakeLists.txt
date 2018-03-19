#=================================================================
# CMake options ==================================================
#=================================================================
cmake_minimum_required(VERSION 2.8)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Include guard ==================================================
#=================================================================
if (DEFINED BANG_PKG_BUILT)
    return()
endif()
set(BANG_PKG_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Check input params =============================================
#=================================================================
include(${CMAKE_CURRENT_LIST_DIR}/../../CMakeIncludes/CMakeCheckInputParams.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Compile Bang library ===========================================
#=================================================================
include(${CMAKE_CURRENT_LIST_DIR}/../../CMakeLists.txt) # Compile Bang into lib
#=================================================================
#=================================================================
#=================================================================

project(BuildPackageBang C CXX)

#=================================================================
# Set variables ==================================================
#=================================================================
set(BANG_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
if (NOT DEFINED PKG_DIRECTORY)
    set(PKG_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BangPackage")
endif()
message("PKG_DIRECTORY: ${PKG_DIRECTORY}")
set(LIBRARY_OUTPUT_DIRECTORY "${PKG_DIRECTORY}/Libraries")
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Create dirs if needed ==========================================
#=================================================================
add_custom_command(
  OUTPUT ${PKG_DIRECTORY}
  COMMAND mkdir -p ${PKG_DIRECTORY}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Creating ${PKG_DIRECTORY} dir..."
  VERBATIM
 )
add_custom_command(
  OUTPUT ${LIBRARY_OUTPUT_DIRECTORY}
  COMMAND mkdir -p ${LIBRARY_OUTPUT_DIRECTORY}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Creating ${LIBRARY_OUTPUT_DIRECTORY} dir..."
  DEPENDS ${PKG_DIRECTORY}
  VERBATIM
 )
add_custom_target(CREATE_PKG_DIRS ALL DEPENDS ${PKG_DIRECTORY} ${LIBRARY_OUTPUT_DIRECTORY})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Copy engine directories ========================================
#=================================================================
# Assets
set(ENGINE_ASSETS_DIR_NAME "Assets")
set(ENGINE_ASSETS_DIR_SRC "${BANG_ROOT}/${ENGINE_ASSETS_DIR_NAME}")
set(ENGINE_ASSETS_DIR_DST "${PKG_DIRECTORY}/${ENGINE_ASSETS_DIR_NAME}")
add_custom_command(
  OUTPUT COPY_ASSETS_DIR_BANG_PKG
  COMMAND /bin/sh -c "cp -r ${ENGINE_ASSETS_DIR_SRC}/* ${ENGINE_ASSETS_DIR_DST}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_ASSETS_DIR_SRC} to ${ENGINE_ASSETS_DIR_DST}..."
  VERBATIM
 )

# Includes
set(ENGINE_INCLUDE_DIR_NAME "include")
set(ENGINE_INCLUDE_DIR_SRC "${BANG_ROOT}/${ENGINE_INCLUDE_DIR_NAME}")
set(ENGINE_INCLUDE_DIR_DST "${PKG_DIRECTORY}/${ENGINE_INCLUDE_DIR_NAME}")
add_custom_command(
  OUTPUT COPY_INCLUDE_DIR_BANG_PKG
  COMMAND /bin/sh -c "cp -r ${ENGINE_INCLUDE_DIR_SRC}/* ${ENGINE_INCLUDE_DIR_DST}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_INCLUDE_DIR_SRC} to ${ENGINE_INCLUDE_DIR_DST}..."
  VERBATIM
 )
add_custom_target(COPY_DIRS ALL DEPENDS
    CREATE_PKG_DIRS
    COPY_ASSETS_DIR_BANG_PKG
    COPY_INCLUDE_DIR_BANG_PKG)
#=================================================================
#=================================================================
#=================================================================
