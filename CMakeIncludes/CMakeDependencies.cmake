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
if (DEFINED BANG_DEPENDENCIES_BUILT)
    return()
endif()
set(BANG_DEPENDENCIES_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Includes =======================================================
#=================================================================
include ("${CMAKE_CURRENT_LIST_DIR}/CMakeMacros.cmake")
#=================================================================
#=================================================================
#=================================================================

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

#=================================================================
# Dependencies ===================================================
#=================================================================

# DL Dependency =============
message("Looking for dependency dl...")
find_library(DL_LIBRARY "dl")
link_libraries(${DL_LIBRARY})

# OGG Dependency =============
message("Looking for dependency ogg...")
find_library(OGG_LIBRARY "ogg")
link_libraries(${OGG_LIBRARY})

# GLX Dependency =============
message("Looking for dependency GLX...")
find_library(GLX_LIBRARY "GLX")
link_libraries(${GLX_LIBRARY})

# GLDISPATCH Dependency =============
message("Looking for dependency GLDISPATCH...")
find_library(GLDISPATCH_LIBRARY "libGLdispatch.so.0")
link_libraries(${GLDISPATCH_LIBRARY})

# MINIZIP Dependency ============================
message("Looking for dependency MINIZIP...")
find_library(MINZIP_LIBRARY "libminizip.so.1")
include_directories(${MINIZIP_INCLUDE_DIRS})
link_libraries(${MINZIP_LIBRARY})

# MINIZIP Dependency ============================
message("Looking for dependency MINIZIP...")
find_library(MINZIP_LIBRARY "libminizip.so.1")
include_directories(${MINIZIP_INCLUDE_DIRS})
link_libraries(${MINZIP_LIBRARY})

# VORBIS Dependency =============
message("Looking for dependency vorbis...")
find_library(VORBIS_LIBRARY "vorbis")
link_libraries(${VORBIS_LIBRARY})

# VORBIS Dependency =============
message("Looking for dependency vorbisenc...")
find_library(VORBISENC_LIBRARY "vorbisenc")
link_libraries(${VORBISENC_LIBRARY})

# FLAC Dependency =============
message("Looking for dependency FLAC...")
find_library(FLAC_LIBRARY "FLAC")
link_libraries(${FLAC_LIBRARY})

# GL Dependency ============================
message("Looking for dependency OpenGL...")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# GLEW Dependency ============================
message("Looking for dependency GLEW...")
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# PNG Dependency ============================
message("Looking for dependency PNG...")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
link_libraries(${PNG_LIBRARIES})

# JPEG Dependency ============================
message("Looking for dependency JPEG...")
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIRS})
link_libraries(${JPEG_LIBRARIES})

# SNDFILE Dependency ============================
message("Looking for dependency sndfile...")
find_library(SNDFILE_LIBRARY "sndfile")
link_libraries(${SNDFILE_LIBRARY})

# SNDIO Dependency =============
message("Looking for dependency sndio...")
find_library(SNDIO_LIBRARY "sndio")
link_libraries(${SNDIO_LIBRARY})

# ASSIMP Dependency ============================
message("Looking for dependency assimp...")
find_library(ASSIMP_LIBRARY "assimp")
link_libraries(${ASSIMP_LIBRARY})

# SDL2 Dependency ============================
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
string(REPLACE "-lpthread" "" SDL2_LIBRARY "${SDL2_LIBRARY}")
#string(STRIP "${SDL2_LIBRARY}" SDL2_LIBRARY)
link_libraries(${SDL2_LIBRARY})
link_libraries(${SDL2_LIBRARIES})

# SDL2_TTF Dependency ============================
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
string(STRIP "${SDL2_TTF_LIBRARY}" SDL2_TTF_LIBRARY)
string(STRIP "${SDL2_TTF_LIBRARIES}" SDL2_TTF_LIBRARIES)
link_libraries(${SDL2_TTF_LIBRARY})
link_libraries(${SDL2_TTF_LIBRARIES})
message("STUFF ${SDL2_LIBRARY}")
message("STUFF ${SDL2_TTF_LIBRARY}")

# OpenAL Dependency ============================
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_libraries(${OPENAL_LIBRARY})

# Needed to solve all undefined references created above!
set(ALL_DEPENDENCIES_LIB_FILES
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${MINZIP_LIBRARY}
    ${GLX_LIBRARY}
    ${GLDISPATCH_LIBRARY}
    ${SNDFILE_LIBRARY}
    ${ASSIMP_LIBRARY}
    ${OPENAL_LIBRARY}
    ${GLEW_LIBRARIES}
    ${DL_LIBRARY}
    ${VORBIS_LIBRARY}
    ${VORBISENC_LIBRARY}
    ${OGG_LIBRARY}    # Must go after vorbis
    ${FLAC_LIBRARY}
    ${SNDIO_LIBRARY}
    ${OPENGL_LIBRARIES}
)

message(${ALL_DEPENDENCIES_LIB_FILES})

#=================================================================
#=================================================================
#=================================================================
