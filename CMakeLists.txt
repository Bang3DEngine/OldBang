# CMake Options
cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE off)

project(Bang C CXX)

set(ROOT ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${ROOT}/src)
set(ENGINE_SRC_DIR ${SRC_DIR}/Engine)

set(CMAKE_BINARY_DIR ${ROOT}/bin)

# In case you want to break on sanitize error write this command on gdb:
# rbreak __asan_report_error

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wl,-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-sanitize-recover")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=leak")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=float-divide-by-zero")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=integer-divide-by-zero")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=return")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=null")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wl,-O3")
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
message("Building in ${CMAKE_BUILD_TYPE} mode...")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(PROJECT_SOURCE_DIR ${SRC_DIR})
list(APPEND CMAKE_MODULE_PATH "${ROOT}/scripts")

# CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wint-to-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--export-dynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DGPROF)
    link_libraries("profiler") # gperftools
endif()

# GLEW Dependency
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# GL Dependency
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

# png Dependency
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
link_libraries(${PNG_LIBRARIES})

# jpeg Dependency
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIRS})
link_libraries(${JPEG_LIBRARIES})

# DL Dependency
link_libraries("dl")

# SndFile Dependency
link_libraries("sndfile")

# Assimp Dependency
link_libraries("assimp")

# SDL Dependency
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
link_libraries(${SDL2_LIBRARY})
link_libraries(${SDL2_LIBRARIES})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
string(STRIP "${SDL2_TTF_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_TTF_LIBRARIES}" SDL2_LIBRARIES)
link_libraries(${SDL2_TTF_LIBRARY})
link_libraries(${SDL2_TTF_LIBRARIES})

# OpenAL Dependency
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_libraries(${OPENAL_LIBRARY})

# Bang include's
SET(BANG_INCLUDE_DIR_ROOT ${ROOT}/include)
SET(BANG_INCLUDE_DIR ${BANG_INCLUDE_DIR_ROOT}/Bang)
include_directories(${BANG_INCLUDE_DIR_ROOT})

# BangCore Library
set(CORE_SRC_FILES
${ENGINE_SRC_DIR}/DataStructures/String.cpp
${ENGINE_SRC_DIR}/Debug/Debug.cpp
${ENGINE_SRC_DIR}/Debug/DebugRenderer.cpp
${ENGINE_SRC_DIR}/Debug/StreamOperators.cpp
${ENGINE_SRC_DIR}/IO/Files/File.cpp
${ENGINE_SRC_DIR}/IO/Files/Path.cpp
${ENGINE_SRC_DIR}/Math/Color.cpp
${ENGINE_SRC_DIR}/Math/Math.cpp
${ENGINE_SRC_DIR}/Math/Plane.cpp
${ENGINE_SRC_DIR}/Math/Random.cpp
${ENGINE_SRC_DIR}/Math/Ray.cpp
${ENGINE_SRC_DIR}/Math/Triangle.cpp
${ENGINE_SRC_DIR}/System/Cursor.cpp
${ENGINE_SRC_DIR}/System/Mutex.cpp
${ENGINE_SRC_DIR}/System/MutexLocker.cpp
${ENGINE_SRC_DIR}/System/SystemProcess.cpp
${ENGINE_SRC_DIR}/System/Thread.cpp
${ENGINE_SRC_DIR}/System/ThreadPool.cpp
)
add_library(BangCoreObjects OBJECT ${CORE_SRC_FILES})
add_library(BangCoreStatic  STATIC $<TARGET_OBJECTS:BangCoreObjects>)
add_library(BangCore        SHARED $<TARGET_OBJECTS:BangCoreObjects>)

macro(add_info_files)
     foreach(_it ${ARGN})
         if(NOT IS_DIRECTORY ${_it})
             get_filename_component(name ${_it} NAME)
             if(NOT ${_it} MATCHES "^/\\\\..*$;~$")
                set_source_files_properties(${_it} PROPERTIES HEADER_FILE_ONLY TRUE)
             endif()
         endif()
     endforeach()
endmacro()
file(GLOB_RECURSE OTHER_FILES "[^.]*.frag" "[^.]*.frag" "[^.]*.frag"
                              "[^.]*.vert" "[^.]*.vert" "[^.]*.vert"
                              "[^.]*.png" "[^.]*.svg" "[^.]*.bmat"
                              "[^.]*.glsl"
                              "${BANG_INCLUDE_DIR}/*.h"
                              "${BANG_INCLUDE_DIR}/*.tcc")
set(OTHER_FILES README.md "${OTHER_FILES}")
add_info_files(${OTHER_FILES})

# BangEngine Library
set (ENGINE_SRC_FILES
${ENGINE_SRC_DIR}/Algorithms/ImageEffects.cpp
${ENGINE_SRC_DIR}/Audio/ALAudioSource.cpp
${ENGINE_SRC_DIR}/Audio/AudioManager.cpp
${ENGINE_SRC_DIR}/Audio/AudioPlayerRunnable.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BangPreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedStruct.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedVariable.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/IReflectable.cpp
${ENGINE_SRC_DIR}/Compiler/CodePreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/Compiler.cpp
${ENGINE_SRC_DIR}/Components/AudioListener.cpp
${ENGINE_SRC_DIR}/Components/AudioSource.cpp
${ENGINE_SRC_DIR}/Components/Behaviour.cpp
${ENGINE_SRC_DIR}/BehaviourContainer.cpp
${ENGINE_SRC_DIR}/Components/Camera.cpp
${ENGINE_SRC_DIR}/Components/Component.cpp
${ENGINE_SRC_DIR}/Components/DirectionalLight.cpp
${ENGINE_SRC_DIR}/Components/Light.cpp
${ENGINE_SRC_DIR}/Components/LineRenderer.cpp
${ENGINE_SRC_DIR}/Components/MeshRenderer.cpp
${ENGINE_SRC_DIR}/Components/PointLight.cpp
${ENGINE_SRC_DIR}/Components/PostProcessEffect.cpp
${ENGINE_SRC_DIR}/Components/RectTransform.cpp
${ENGINE_SRC_DIR}/Components/Renderer.cpp
${ENGINE_SRC_DIR}/Components/Timer.cpp
${ENGINE_SRC_DIR}/Components/Transform.cpp
${ENGINE_SRC_DIR}/Debug/Chrono.cpp
${ENGINE_SRC_DIR}/Debug/FPSChrono.cpp
${ENGINE_SRC_DIR}/Debug/ChronoGL.cpp
${ENGINE_SRC_DIR}/Debug/GEngineDebugger.cpp
${ENGINE_SRC_DIR}/Factory/ComponentFactory.cpp
${ENGINE_SRC_DIR}/Factory/GameObjectFactory.cpp
${ENGINE_SRC_DIR}/Factory/MaterialFactory.cpp
${ENGINE_SRC_DIR}/Factory/MeshFactory.cpp
${ENGINE_SRC_DIR}/Factory/IconManager.cpp
${ENGINE_SRC_DIR}/GameObjects/GameObject.cpp
${ENGINE_SRC_DIR}/GameObjects/Scene.cpp
${ENGINE_SRC_DIR}/General/Application.cpp
${ENGINE_SRC_DIR}/General/BehaviourManager.cpp
${ENGINE_SRC_DIR}/General/FileTracker.cpp
${ENGINE_SRC_DIR}/General/GUID.cpp
${ENGINE_SRC_DIR}/General/IGUIDable.cpp
${ENGINE_SRC_DIR}/General/ICloneable.cpp
${ENGINE_SRC_DIR}/General/IEventEmitter.cpp
${ENGINE_SRC_DIR}/General/IEventListener.cpp
${ENGINE_SRC_DIR}/General/IDestroyListener.cpp
${ENGINE_SRC_DIR}/General/Input.cpp
${ENGINE_SRC_DIR}/General/SceneManager.cpp
${ENGINE_SRC_DIR}/General/Object.cpp
${ENGINE_SRC_DIR}/General/ObjectId.cpp
${ENGINE_SRC_DIR}/General/Selection.cpp
${ENGINE_SRC_DIR}/General/Time.cpp
${ENGINE_SRC_DIR}/Gizmos/Gizmos.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/Framebuffer.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/GBuffer.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/IUniformBuffer.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/SelectionFramebuffer.cpp
${ENGINE_SRC_DIR}/Graphics/General/AspectRatioMode.cpp
${ENGINE_SRC_DIR}/Graphics/General/Font.cpp
${ENGINE_SRC_DIR}/Graphics/General/FontSheetCreator.cpp
${ENGINE_SRC_DIR}/Graphics/General/Image.cpp
${ENGINE_SRC_DIR}/Graphics/General/Material.cpp
${ENGINE_SRC_DIR}/Graphics/General/Texture.cpp
${ENGINE_SRC_DIR}/Graphics/General/Texture2D.cpp
${ENGINE_SRC_DIR}/Graphics/General/TextureUnitManager.cpp
${ENGINE_SRC_DIR}/Graphics/General/VAO.cpp
${ENGINE_SRC_DIR}/Graphics/General/VBO.cpp
${ENGINE_SRC_DIR}/Graphics/General/GL.cpp
${ENGINE_SRC_DIR}/Graphics/General/GLUniforms.cpp
${ENGINE_SRC_DIR}/Graphics/General/GLObject.cpp
${ENGINE_SRC_DIR}/Graphics/GEngine/GEngine.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/Shader.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/ShaderPreprocessor.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/ShaderProgram.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/ShaderProgramFactory.cpp
${ENGINE_SRC_DIR}/ImportFiles/GUIDManager.cpp
${ENGINE_SRC_DIR}/ImportFiles/ImportFilesManager.cpp
${ENGINE_SRC_DIR}/IO/General/Paths.cpp
${ENGINE_SRC_DIR}/IO/Files/Extensions.cpp
${ENGINE_SRC_DIR}/IO/Files/Images/ImageIO.cpp
${ENGINE_SRC_DIR}/IO/Files/Images/ImageIODDS.cpp
${ENGINE_SRC_DIR}/IO/Files/Images/nv_dds.cpp
${ENGINE_SRC_DIR}/IO/Files/ModelIO.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLAttribute.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLNode.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLNodeReader.cpp
${ENGINE_SRC_DIR}/Math/AABox.cpp
${ENGINE_SRC_DIR}/Math/Sphere.cpp
${ENGINE_SRC_DIR}/Resources/Asset.cpp
${ENGINE_SRC_DIR}/Resources/AudioClip.cpp
${ENGINE_SRC_DIR}/Resources/Mesh.cpp
${ENGINE_SRC_DIR}/Resources/Model.cpp
${ENGINE_SRC_DIR}/Resources/Prefab.cpp
${ENGINE_SRC_DIR}/Resources/Resource.cpp
${ENGINE_SRC_DIR}/Resources/Resources.cpp
${ENGINE_SRC_DIR}/Resources/ResourceHandle.cpp
${ENGINE_SRC_DIR}/Resources/Serializable.cpp
${ENGINE_SRC_DIR}/System/Library.cpp
${ENGINE_SRC_DIR}/System/SystemUtils.cpp
${ENGINE_SRC_DIR}/System/SystemClipboard.cpp
${ENGINE_SRC_DIR}/Text/TextFormatter.cpp
${ENGINE_SRC_DIR}/UIComponents/IFocusable.cpp
${ENGINE_SRC_DIR}/UIComponents/ILayoutController.cpp
${ENGINE_SRC_DIR}/UIComponents/ILayoutElement.cpp
${ENGINE_SRC_DIR}/UIComponents/UIAutoFocuser.cpp
${ENGINE_SRC_DIR}/UIComponents/UIButton.cpp
${ENGINE_SRC_DIR}/UIComponents/UICanvas.cpp
${ENGINE_SRC_DIR}/UIComponents/UICheckBox.cpp
${ENGINE_SRC_DIR}/UIComponents/UIComboBox.cpp
${ENGINE_SRC_DIR}/UIComponents/UIContentSizeFitter.cpp
${ENGINE_SRC_DIR}/UIComponents/UIAspectRatioFitter.cpp
${ENGINE_SRC_DIR}/UIComponents/UIDirLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIFileList.cpp
${ENGINE_SRC_DIR}/UIComponents/UIFocusable.cpp
${ENGINE_SRC_DIR}/UIComponents/UIFocusable.cpp
${ENGINE_SRC_DIR}/UIComponents/UIGridLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIGroupLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIHorizontalLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIImageRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UIInputText.cpp
${ENGINE_SRC_DIR}/UIComponents/UIInputNumber.cpp
${ENGINE_SRC_DIR}/UIComponents/UILayoutIgnorer.cpp
${ENGINE_SRC_DIR}/UIComponents/UILayoutElement.cpp
${ENGINE_SRC_DIR}/UIComponents/UILayoutManager.cpp
${ENGINE_SRC_DIR}/UIComponents/UILabel.cpp
${ENGINE_SRC_DIR}/UIComponents/UIList.cpp
${ENGINE_SRC_DIR}/UIComponents/UIMask.cpp
${ENGINE_SRC_DIR}/UIComponents/UIRectMask.cpp
${ENGINE_SRC_DIR}/UIComponents/UIRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UIRendererCacher.cpp
${ENGINE_SRC_DIR}/UIComponents/UIScrollArea.cpp
${ENGINE_SRC_DIR}/UIComponents/UIScrollBar.cpp
${ENGINE_SRC_DIR}/UIComponents/UIScrollPanel.cpp
${ENGINE_SRC_DIR}/UIComponents/UISlider.cpp
${ENGINE_SRC_DIR}/UIComponents/UITextCursor.cpp
${ENGINE_SRC_DIR}/UIComponents/UITextRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UITree.cpp
${ENGINE_SRC_DIR}/UIComponents/UIVerticalLayout.cpp
${ENGINE_SRC_DIR}/Windows/Dialog.cpp
${ENGINE_SRC_DIR}/Windows/DialogWindow.cpp
${ENGINE_SRC_DIR}/Windows/Window.cpp
${ENGINE_SRC_DIR}/Windows/WindowManager.cpp
${OTHER_FILES}
)
add_library(BangEngineObjects OBJECT ${ENGINE_SRC_FILES})

list(APPEND ALL_SRC_FILES ${CORE_SRC_FILES})
list(APPEND ALL_SRC_FILES ${GRAPHICS_SRC_FILES})
list(APPEND ALL_SRC_FILES ${ENGINE_SRC_FILES})
add_library(BangStaticLib STATIC $<TARGET_OBJECTS:BangCoreObjects>
                                 $<TARGET_OBJECTS:BangEngineObjects>)
add_library(BangSharedLib SHARED $<TARGET_OBJECTS:BangCoreObjects>
                                 $<TARGET_OBJECTS:BangEngineObjects>)
set_target_properties(BangSharedLib PROPERTIES OUTPUT_NAME Bang)


# Executables ===========================
link_directories(${LIBRARY_OUTPUT_PATH})
link_libraries("Bang") # Use installed lib from now on
set(PREPROCESSOR_SRC_FILES
${SRC_DIR}/MainBangPreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BangPreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedStruct.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedVariable.cpp
)
add_executable(BangPreprocessor ${PREPROCESSOR_SRC_FILES})
add_dependencies(BangPreprocessor BangEngineObjects)
