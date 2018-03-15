list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
message(${CMAKE_CURRENT_LIST_DIR})

#=================================================================
# Dependencies ===================================================
#=================================================================

# GLEW Dependency ============================
message("Looking for dependency GLEW...")
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# GL Dependency ============================
message("Looking for dependency OpenGL...")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# png Dependency ============================
message("Looking for dependency PNG...")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
link_libraries(${PNG_LIBRARIES})

# jpeg Dependency ============================
message("Looking for dependency JPEG...")
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIRS})
link_libraries(${JPEG_LIBRARIES})

# DL Dependency ============================
message("Looking for dependency dl...")
find_library(DL_LIBRARY "dl")
link_libraries(${DL_LIBRARY})

# SndFile Dependency ============================
message("Looking for dependency sndfile...")
find_library(SNDFILE_LIBRARY "sndfile")
link_libraries(${SNDFILE_LIBRARY})

# Assimp Dependency ============================
message("Looking for dependency assimp...")
find_library(ASSIMP_LIBRARY "assimp")
link_libraries(${ASSIMP_LIBRARY})

# SDL2 Dependency ============================
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
link_libraries(${SDL2_LIBRARY})
link_libraries(${SDL2_LIBRARIES})

# SDL2_TTF Dependency ============================
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
string(STRIP "${SDL2_TTF_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_TTDependencyF_LIBRARIES}" SDL2_LIBRARIES)
link_libraries(${SDL2_TTF_LIBRARY})
link_libraries(${SDL2_TTF_LIBRARIES})

# OpenAL Dependency ============================
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_libraries(${OPENAL_LIBRARY})

# All dependencies list
set(ALL_DEPENDENCIES_LIB_FILES
${GLEW_LIBRARIES};
${OPENGL_LIBRARIES};
${PNG_LIBRARIES};
${JPEG_LIBRARIES};
${SDL2_LIBRARY};
${SDL2_LIBRARIES};
${SDL2_TTF_LIBRARY};
${SDL2_TTF_LIBRARIES};
${DL_LIBRARY};
${SNDFILE_LIBRARY};
${ASSIMP_LIBRARY};
${OPENAL_LIBRARY}
)

#=================================================================
#=================================================================
#=================================================================
