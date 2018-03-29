#=================================================================
# CMake options ==================================================
#=================================================================
cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)
project(BangThirdParty C CXX)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Variables ======================================================
#=================================================================
# General
set(NUM_COMPILE_THREADS 4)
add_definitions(-DBUILD_STATIC)
# set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/CMakeIncludes")
#
# #=================================================================
# # Shared dependencies ============================================
# #=================================================================
# # VORBIS Dependency ============================
# message("Looking for dependency VORBIS...")
# find_package(OggVorbis REQUIRED)
# # include_directories(${VORBIS_INCLUDE_DIRS})
# link_libraries(${VORBIS_LIBRARY})
#
# # FLAC Dependency ============================
# message("Looking for dependency FLAC...")
# find_package(FLAC REQUIRED)
# # include_directories(${FLAC_INCLUDE_DIRS})
# link_libraries(${FLAC_LIBRARY})
#
# link_libraries()
# #=================================================================
# #=================================================================
# #=================================================================

# Assimp
set(ASSIMP_DIR "${CMAKE_CURRENT_LIST_DIR}/assimp")
set(ASSIMP_INCLUDE_DIR_0 "${ASSIMP_DIR}/include")
set(ASSIMP_INCLUDE_DIR_1 "${ASSIMP_DIR}/build/include")
set(ASSIMP_LIB_OUTPUT_DIR  "${ASSIMP_DIR}/build/code")
set(ASSIMP_LIB_OUTPUT_NAME "libassimp.a")
set(ASSIMP_LIB_OUTPUT "${ASSIMP_LIB_OUTPUT_DIR}/${ASSIMP_LIB_OUTPUT_NAME}")

# IRRXML (used by Assimp)
set(IRRXML_DIR "${ASSIMP_DIR}/build/contrib/irrXML")
set(IRRXML_LIB_OUTPUT_DIR  "${IRRXML_DIR}")
set(IRRXML_LIB_OUTPUT_NAME "libIrrXML.a")
set(IRRXML_LIB_OUTPUT "${IRRXML_LIB_OUTPUT_DIR}/${IRRXML_LIB_OUTPUT_NAME}")

# ZLIB (used by Assimp, and using the one inside it ftm)
set(ZLIB_DIR "${ASSIMP_DIR}/build/contrib/zlib")
set(ZLIB_LIB_OUTPUT_DIR  "${ZLIB_DIR}")
set(ZLIB_LIB_OUTPUT_NAME "libzlibstatic.a")
set(ZLIB_LIB_OUTPUT "${ZLIB_LIB_OUTPUT_DIR}/${ZLIB_LIB_OUTPUT_NAME}")

# Freetype2
set(FREETYPE_DIR "${CMAKE_CURRENT_LIST_DIR}/freetype-2.4.0")
set(FREETYPE_INSTALL_DIR_NAME "freetype-install")
set(FREETYPE_INSTALL_DIR "${FREETYPE_DIR}/${FREETYPE_INSTALL_DIR_NAME}")
set(FREETYPE_LIB_OUTPUT_DIR  "${FREETYPE_INSTALL_DIR}/lib")
set(FREETYPE_LIB_OUTPUT_NAME "libfreetype.a")
set(FREETYPE_LIB_OUTPUT  "${FREETYPE_LIB_OUTPUT_DIR}/${FREETYPE_LIB_OUTPUT_NAME}")

# GLEW
set(GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/glew-2.1.0")
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
set(GLEW_LIB_OUTPUT_DIR  "${GLEW_DIR}/lib")
set(GLEW_LIB_OUTPUT_NAME "libGLEW.a")
set(GLEW_LIB_OUTPUT  "${GLEW_LIB_OUTPUT_DIR}/${GLEW_LIB_OUTPUT_NAME}")

# JPEG
set(JPEG_DIR "${CMAKE_CURRENT_LIST_DIR}/libjpeg-turbo")
set(JPEG_INCLUDE_DIR "${JPEG_DIR}")
set(JPEG_LIB_OUTPUT_DIR  "${JPEG_DIR}/build")
set(JPEG_LIB_OUTPUT_NAME "libturbojpeg.a")
set(JPEG_LIB_OUTPUT  "${JPEG_LIB_OUTPUT_DIR}/${JPEG_LIB_OUTPUT_NAME}")

# PNG
set(PNG_DIR "${CMAKE_CURRENT_LIST_DIR}/libpng-1.6.34")
set(PNG_INCLUDE_DIR_0 "${PNG_DIR}")
set(PNG_INCLUDE_DIR_1 "${PNG_DIR}/build")
set(PNG_LIB_OUTPUT_DIR  "${PNG_DIR}/build")
set(PNG_LIB_OUTPUT_NAME "libpng.a")
set(PNG_LIB_OUTPUT  "${PNG_LIB_OUTPUT_DIR}/${PNG_LIB_OUTPUT_NAME}")

# OGG
set(OGG_DIR "${CMAKE_CURRENT_LIST_DIR}/ogg")
set(OGG_LIB_OUTPUT_DIR  "${OGG_DIR}/build")
set(OGG_LIB_OUTPUT_NAME "libogg.a")
set(OGG_LIB_OUTPUT  "${OGG_LIB_OUTPUT_DIR}/${OGG_LIB_OUTPUT_NAME}")

# FLAC
set(FLAC_DIR "${CMAKE_CURRENT_LIST_DIR}/flac")
set(FLAC_LIB_OUTPUT_DIR  "${FLAC_DIR}/src/libFLAC/.libs")
set(FLAC_LIB_OUTPUT_NAME "libFLAC-static.a")
set(FLAC_LIB_OUTPUT  "${FLAC_LIB_OUTPUT_DIR}/${FLAC_LIB_OUTPUT_NAME}")

# VORBIS
set(VORBIS_DIR "${CMAKE_CURRENT_LIST_DIR}/vorbis")
set(VORBIS_LIB_OUTPUT_DIR  "${VORBIS_DIR}/build/lib")
set(VORBIS_LIB_OUTPUT_NAME_0 "libvorbis.a")
set(VORBIS_LIB_OUTPUT_NAME_1 "libvorbisenc.a")
set(VORBIS_LIB_OUTPUT_NAME_2 "libvorbisfile.a")
set(VORBIS_LIB_OUTPUT_0 "${VORBIS_LIB_OUTPUT_DIR}/${VORBIS_LIB_OUTPUT_NAME_0}")
set(VORBIS_LIB_OUTPUT_1 "${VORBIS_LIB_OUTPUT_DIR}/${VORBIS_LIB_OUTPUT_NAME_1}")
set(VORBIS_LIB_OUTPUT_2 "${VORBIS_LIB_OUTPUT_DIR}/${VORBIS_LIB_OUTPUT_NAME_2}")

# SNDFILE
set(SNDFILE_DIR "${CMAKE_CURRENT_LIST_DIR}/libsndfile")
set(SNDFILE_LIB_OUTPUT_DIR  "${SNDFILE_DIR}/src/.libs")
set(SNDFILE_LIB_OUTPUT_NAME "libsndfile.a")
set(SNDFILE_LIB_OUTPUT  "${SNDFILE_LIB_OUTPUT_DIR}/${SNDFILE_LIB_OUTPUT_NAME}")

# ALSAOBJS
set(ALSAOBJS_DIR "${CMAKE_CURRENT_LIST_DIR}/alsa-lib-1.1.4")
set(ALSAOBJS_LIB_OUTPUT_DIR  "${ALSAOBJS_DIR}/build/lib")
set(ALSAOBJS_LIB_OUTPUT_NAME "alsaobjs.done")
set(ALSAOBJS_LIB_OUTPUT  "${ALSAOBJS_LIB_OUTPUT_DIR}/${ALSAOBJS_LIB_OUTPUT_NAME}")
# file(REMOVE ${ALSAOBJS_LIB_OUTPUT_NAME})

# SNDIO
set(SNDIO_DIR "${CMAKE_CURRENT_LIST_DIR}/sndio")
set(SNDIO_LIB_OUTPUT_DIR  "${SNDIO_DIR}/libsndio")
set(SNDIO_LIB_OUTPUT_NAME "libsndio.a")
set(SNDIO_LIB_OUTPUT  "${SNDIO_LIB_OUTPUT_DIR}/${SNDIO_LIB_OUTPUT_NAME}")

# OPENAL
set(OPENAL_DIR "${CMAKE_CURRENT_LIST_DIR}/openal-soft")
set(OPENAL_INCLUDE_DIR "${OPENAL_DIR}/include")
set(OPENAL_LIB_OUTPUT_DIR  "${OPENAL_DIR}/build")
set(OPENAL_LIB_OUTPUT_NAME "libopenal.a")
set(OPENAL_LIB_OUTPUT  "${OPENAL_LIB_OUTPUT_DIR}/${OPENAL_LIB_OUTPUT_NAME}")

# SDL2
set(SDL2_DIR "${CMAKE_CURRENT_LIST_DIR}/SDL2-2.0.8")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
set(SDL2_LIB_OUTPUT_DIR  "${SDL2_DIR}/build")
set(SDL2_LIB_OUTPUT_NAME "libSDL2.a")
set(SDL2_LIB_OUTPUT  "${SDL2_LIB_OUTPUT_DIR}/${SDL2_LIB_OUTPUT_NAME}")

# SDL2_ttf
set(SDL2TTF_DIR "${CMAKE_CURRENT_LIST_DIR}/SDL2_ttf-2.0.14")
set(SDL2TTF_INCLUDE_DIR "${SDL2TTF_DIR}")
set(SDL2TTF_LIB_OUTPUT_DIR  "${SDL2TTF_DIR}/.libs")
set(SDL2TTF_LIB_OUTPUT_NAME "libSDL2_ttf.a")
set(SDL2TTF_LIB_OUTPUT  "${SDL2TTF_LIB_OUTPUT_DIR}/${SDL2TTF_LIB_OUTPUT_NAME}")
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Assimp Compile =================================================
#=================================================================
add_custom_command(
  OUTPUT ${ASSIMP_LIB_OUTPUT}
  COMMAND
      cd ${ASSIMP_DIR} && mkdir -p build && cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF
            -DSYSTEM_IRRXML=OFF
            -DASSIMP_BUILD_ZLIB=ON
            -DASSIMP_BUILD_TESTS=OFF
            -DASSIMP_BUILD_SAMPLES=OFF
            -DASSIMP_NO_EXPORT=OFF
            -BUILD_FRAMEWORK=OFF
            -DASSIMP_BUILD_ASSIMP_TOOLS=ON .. &&
      make -j${NUM_COMPILE_THREADS}
      VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# IRRXML Compile =================================================
#=================================================================
add_custom_command(
  OUTPUT ${IRRXML_LIB_OUTPUT}
  COMMAND cp ${IRRXML_LIB_OUTPUT} ${IRRXML_LIB_OUTPUT}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================


#=================================================================
# ZLib Compile ===================================================
#=================================================================
add_custom_command(
  OUTPUT ${ZLIB_LIB_OUTPUT}
  COMMAND cp ${ZLIB_LIB_OUTPUT} ${ZLIB_LIB_OUTPUT}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Freetype Compile ===============================================
#=================================================================
add_custom_command(
  OUTPUT ${FREETYPE_LIB_OUTPUT}
  COMMAND
      cd ${FREETYPE_DIR} &&
      sh autogen.sh &&
      mkdir -p ${FREETYPE_INSTALL_DIR} &&
      ./configure --prefix=${FREETYPE_INSTALL_DIR} &&
      make -j${NUM_COMPILE_THREADS} &&
      make install
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# GLEW Compile ===================================================
#=================================================================
add_custom_command(
  OUTPUT ${GLEW_LIB_OUTPUT}
  COMMAND
      cd ${GLEW_DIR} &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# JPEG Compile ===================================================
#=================================================================
add_custom_command(
  OUTPUT ${JPEG_LIB_OUTPUT}
  COMMAND
      cd ${JPEG_DIR} &&
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF -DWITH_SIMD=OFF -DREQUIRE_SIMD=OFF .. &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# PNG Compile ====================================================
#=================================================================
add_custom_command(
  OUTPUT ${PNG_LIB_OUTPUT}
  COMMAND
      cd ${PNG_DIR} &&
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF .. &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# OGG Compile ====================================================
#=================================================================
add_custom_command(
  OUTPUT ${OGG_LIB_OUTPUT}
  COMMAND
      cd ${OGG_DIR} &&
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF .. &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# VORBIS Compile =================================================
#=================================================================
add_custom_command(
  OUTPUT ${VORBIS_LIB_OUTPUT_0} ${VORBIS_LIB_OUTPUT_1} ${VORBIS_LIB_OUTPUT_2}
  COMMAND
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF .. &&
      make -j6
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# FLAC Compile ====================================================
#=================================================================
add_custom_command(
  OUTPUT ${FLAC_LIB_OUTPUT}
  COMMAND
      sudo ln -s /usr/bin/libtoolize /usr/bin/libtool &&
      sh autogen.sh &&
      mkdir -p flac-install &&
      ./configure --prefix=$(pwd)/flac-install &&
      make -j6 &&
      make install
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================


#=================================================================
# ALSAOBJS Compile ===============================================
#=================================================================
add_custom_command(
  OUTPUT ${ALSAOBJS_LIB_OUTPUT}
  EXECUTE_PROCESS(
      COMMAND
          cd ${ALSAOBJS_DIR} &&
          ./configure --prefix=${ALSAOBJS_DIR}/build --enable-shared=no --enable-static=yes &&
          make -j${NUM_COMPILE_THREADS} VERBOSE=1 &&
          make install VERBOSE=1 &&
          cd build/lib &&
          ar x libasound.a &&
          /bin/sh -c "cp -R ${ALSAOBJS_DIR}/src/*.o     ${ALSAOBJS_DIR}/build/lib" &&
          /bin/sh -c "cp -R ${ALSAOBJS_DIR}/src/pcm/*.o ${ALSAOBJS_DIR}/build/lib" &&
          /bin/sh -c "cp -R ${ALSAOBJS_DIR}/src/ucm/*.o ${ALSAOBJS_DIR}/build/lib"
  )
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# SNDIO Compile ==================================================
#=================================================================
add_custom_command(
  OUTPUT ${SNDIO_LIB_OUTPUT}
  COMMAND
      cd ${SNDIO_DIR} &&
      ./configure --prefix=build &&
      make -j${NUM_COMPILE_THREADS} &&
      make install &&
      cd libsndio &&
      /bin/sh -c "ar ru ${SNDIO_DIR}/libsndio/${SNDIO_LIB_OUTPUT_NAME} ${SNDIO_DIR}/libsndio/*.o ${ALSAOBJS_LIB_OUTPUT_DIR}/*.o"
  DEPENDS ${ALSAOBJS_LIB_OUTPUT}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# SNDFILE Compile ================================================
#=================================================================
add_custom_command(
  OUTPUT ${SNDFILE_LIB_OUTPUT}
  COMMAND
      cd ${SNDFILE_DIR} &&
      sh autogen.sh &&
      ./configure &&
      make -j${NUM_COMPILE_THREADS}
  DEPENDS ${SNDIO_LIB_OUTPUT} ${ALSAOBJS_LIB_OUTPUT}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# OPENAL Compile =================================================
#=================================================================
add_custom_command(
  OUTPUT ${OPENAL_LIB_OUTPUT}
  COMMAND
      cd ${OPENAL_DIR} &&
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF
          -DLIBTYPE=STATIC
          .. &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# SDL2 Compile ===================================================
#=================================================================
add_custom_target(TARGET_SDL2)
add_custom_command(
  OUTPUT ${SDL2_LIB_OUTPUT}
  COMMAND
      cd ${SDL2_DIR} &&
      mkdir -p build &&
      cd build &&
      cmake -DBUILD_SHARED_LIBS=OFF .. &&
      make -j${NUM_COMPILE_THREADS}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# SDL2TTF Compile ================================================
#=================================================================
add_custom_command(
  OUTPUT ${SDL2TTF_LIB_OUTPUT}
  COMMAND
      cd ${SDL2TTF_DIR} &&
      sh autogen.sh &&
      ./configure --with-freetype-prefix=${FREETYPE_INSTALL_DIR} &&
      make -j${NUM_COMPILE_THREADS}
  DEPENDS ${FREETYPE_LIB_OUTPUT}
  VERBATIM
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Variables ======================================================
#=================================================================
set(ALL_DEPENDENCIES_LIB_FILES
    ${ALL_DEPENDENCIES_LIB_FILES}
    ${ASSIMP_INCLUDE_DIR_0}
    ${ASSIMP_INCLUDE_DIR_1}
    ${IRRXML_LIB_OUTPUT}
    ${JPEG_LIB_OUTPUT}
    ${PNG_LIB_OUTPUT}
    ${SNDIO_LIB_OUTPUT}
    ${SNDFILE_LIB_OUTPUT}
    ${OPENAL_LIB_OUTPUT}
    ${SDL2_LIB_OUTPUT}
    ${SDL2TTF_LIB_OUTPUT}
    ${SNDIO_LIB_OUTPUT}
    ${OGG_LIB_OUTPUT}
    ${VORBIS_LIB_OUTPUT_0}
    ${VORBIS_LIB_OUTPUT_1}
    ${VORBIS_LIB_OUTPUT_2}
    ${FLAC_LIB_OUTPUT}
    ${GLEW_LIB_OUTPUT}
    ${FREETYPE_LIB_OUTPUT} # Must go after sdl2ttf
    ${ZLIB_LIB_OUTPUT}     # Must go after libpng, libassimp
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Include dirs ===================================================
#=================================================================
include_directories(
    ${GLEW_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR_0}
    ${PNG_INCLUDE_DIR_1}
    ${SDL2_INCLUDE_DIR}
    ${SDL2TTF_INCLUDE_DIR}
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Add Dummy target ===============================================
#=================================================================
add_custom_target(Dummy ALL DEPENDS ${ALL_DEPENDENCIES_LIB_FILES})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Check Force rebuild ============================================
#=================================================================
if (${FORCE_REBUILD}) # If force rebuild, remove all outputlibs
    foreach(OUTPUT_LIB_FILE ${ALL_DEPENDENCIES_LIB_FILES})
        file(REMOVE ${OUTPUT_LIB_FILE})
    endforeach()
endif()
#=================================================================
#=================================================================
#=================================================================

