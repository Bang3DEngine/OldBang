# CMake Options
cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE off)

project(Bang C CXX)

if(NOT DEFINED BANG_DIR)
    set(BANG_DIR ${CMAKE_SOURCE_DIR})
endif()
set(BANG_SRC_DIR ${BANG_DIR}/src)
set(BANG_ENGINE_SRC_DIR ${BANG_SRC_DIR}/Engine)

set(CMAKE_BINARY_DIR ${BANG_DIR}/bin)

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
set(PROJECT_SOURCE_DIR ${BANG_SRC_DIR})
list(APPEND CMAKE_MODULE_PATH "${BANG_DIR}/scripts")

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
SET(BANG_INCLUDE_DIR_BANG_DIR ${BANG_DIR}/include)
SET(BANG_INCLUDE_DIR ${BANG_INCLUDE_DIR_BANG_DIR}/Bang)
include_directories(${BANG_INCLUDE_DIR_BANG_DIR})

# Macro to add misc files
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
${BANG_ENGINE_SRC_DIR}/Algorithms/ImageEffects.cpp
${BANG_ENGINE_SRC_DIR}/Audio/ALAudioSource.cpp
${BANG_ENGINE_SRC_DIR}/Audio/AudioManager.cpp
${BANG_ENGINE_SRC_DIR}/Audio/AudioPlayerRunnable.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BangPreprocessor.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedStruct.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedVariable.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/BangPreprocessor/IReflectable.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/CodePreprocessor.cpp
${BANG_ENGINE_SRC_DIR}/Compiler/Compiler.cpp
${BANG_ENGINE_SRC_DIR}/Components/AudioListener.cpp
${BANG_ENGINE_SRC_DIR}/Components/AudioSource.cpp
${BANG_ENGINE_SRC_DIR}/Components/Behaviour.cpp
${BANG_ENGINE_SRC_DIR}/BehaviourContainer.cpp
${BANG_ENGINE_SRC_DIR}/Components/Camera.cpp
${BANG_ENGINE_SRC_DIR}/Components/Component.cpp
${BANG_ENGINE_SRC_DIR}/Components/DirectionalLight.cpp
${BANG_ENGINE_SRC_DIR}/Components/Light.cpp
${BANG_ENGINE_SRC_DIR}/Components/LineRenderer.cpp
${BANG_ENGINE_SRC_DIR}/Components/MeshRenderer.cpp
${BANG_ENGINE_SRC_DIR}/Components/PointLight.cpp
${BANG_ENGINE_SRC_DIR}/Components/PostProcessEffect.cpp
${BANG_ENGINE_SRC_DIR}/Components/RectTransform.cpp
${BANG_ENGINE_SRC_DIR}/Components/Renderer.cpp
${BANG_ENGINE_SRC_DIR}/Components/Timer.cpp
${BANG_ENGINE_SRC_DIR}/Components/Transform.cpp
${BANG_ENGINE_SRC_DIR}/Components/PostProcessEffects/PostProcessEffectSSAO.cpp
${BANG_ENGINE_SRC_DIR}/DataStructures/String.cpp
${BANG_ENGINE_SRC_DIR}/Debug/Debug.cpp
${BANG_ENGINE_SRC_DIR}/Debug/DebugRenderer.cpp
${BANG_ENGINE_SRC_DIR}/Debug/StreamOperators.cpp
${BANG_ENGINE_SRC_DIR}/Debug/Chrono.cpp
${BANG_ENGINE_SRC_DIR}/Debug/FPSChrono.cpp
${BANG_ENGINE_SRC_DIR}/Debug/ChronoGL.cpp
${BANG_ENGINE_SRC_DIR}/Debug/GEngineDebugger.cpp
${BANG_ENGINE_SRC_DIR}/Factory/ComponentFactory.cpp
${BANG_ENGINE_SRC_DIR}/Factory/GameObjectFactory.cpp
${BANG_ENGINE_SRC_DIR}/Factory/MaterialFactory.cpp
${BANG_ENGINE_SRC_DIR}/Factory/MeshFactory.cpp
${BANG_ENGINE_SRC_DIR}/Factory/IconManager.cpp
${BANG_ENGINE_SRC_DIR}/GameObjects/GameObject.cpp
${BANG_ENGINE_SRC_DIR}/GameObjects/Scene.cpp
${BANG_ENGINE_SRC_DIR}/General/Application.cpp
${BANG_ENGINE_SRC_DIR}/General/BehaviourManager.cpp
${BANG_ENGINE_SRC_DIR}/General/FileTracker.cpp
${BANG_ENGINE_SRC_DIR}/General/GUID.cpp
${BANG_ENGINE_SRC_DIR}/General/IGUIDable.cpp
${BANG_ENGINE_SRC_DIR}/General/ICloneable.cpp
${BANG_ENGINE_SRC_DIR}/General/IEventEmitter.cpp
${BANG_ENGINE_SRC_DIR}/General/IEventListener.cpp
${BANG_ENGINE_SRC_DIR}/General/IDestroyListener.cpp
${BANG_ENGINE_SRC_DIR}/General/Input.cpp
${BANG_ENGINE_SRC_DIR}/General/Object.cpp
${BANG_ENGINE_SRC_DIR}/General/ObjectId.cpp
${BANG_ENGINE_SRC_DIR}/General/SceneManager.cpp
${BANG_ENGINE_SRC_DIR}/General/Selection.cpp
${BANG_ENGINE_SRC_DIR}/General/Settings.cpp
${BANG_ENGINE_SRC_DIR}/General/Time.cpp
${BANG_ENGINE_SRC_DIR}/Gizmos/Gizmos.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Buffers/Framebuffer.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Buffers/GBuffer.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Buffers/IUniformBuffer.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Buffers/SelectionFramebuffer.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/AspectRatioMode.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/Font.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/FontSheetCreator.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/Image.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/Material.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/ShadowMapper.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/Texture.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/Texture2D.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/TextureUnitManager.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/VAO.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/VBO.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/GL.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/GLUniforms.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/General/GLObject.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/GEngine/GEngine.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Shaders/Shader.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Shaders/ShaderPreprocessor.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Shaders/ShaderProgram.cpp
${BANG_ENGINE_SRC_DIR}/Graphics/Shaders/ShaderProgramFactory.cpp
${BANG_ENGINE_SRC_DIR}/ImportFiles/GUIDManager.cpp
${BANG_ENGINE_SRC_DIR}/ImportFiles/ImportFilesManager.cpp
${BANG_ENGINE_SRC_DIR}/IO/General/Paths.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/Extensions.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/File.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/Images/ImageIO.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/Images/ImageIODDS.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/Images/nv_dds.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/Path.cpp
${BANG_ENGINE_SRC_DIR}/IO/Files/ModelIO.cpp
${BANG_ENGINE_SRC_DIR}/IO/XML/XMLAttribute.cpp
${BANG_ENGINE_SRC_DIR}/IO/XML/XMLNode.cpp
${BANG_ENGINE_SRC_DIR}/IO/XML/XMLNodeReader.cpp
${BANG_ENGINE_SRC_DIR}/Math/AABox.cpp
${BANG_ENGINE_SRC_DIR}/Math/Sphere.cpp
${BANG_ENGINE_SRC_DIR}/Math/Color.cpp
${BANG_ENGINE_SRC_DIR}/Math/Geometry.cpp
${BANG_ENGINE_SRC_DIR}/Math/Math.cpp
${BANG_ENGINE_SRC_DIR}/Math/Plane.cpp
${BANG_ENGINE_SRC_DIR}/Math/Quad.cpp
${BANG_ENGINE_SRC_DIR}/Math/Random.cpp
${BANG_ENGINE_SRC_DIR}/Math/Ray.cpp
${BANG_ENGINE_SRC_DIR}/Math/Triangle.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Asset.cpp
${BANG_ENGINE_SRC_DIR}/Resources/AudioClip.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Mesh.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Model.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Prefab.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Resource.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Resources.cpp
${BANG_ENGINE_SRC_DIR}/Resources/ResourceHandle.cpp
${BANG_ENGINE_SRC_DIR}/Resources/Serializable.cpp
${BANG_ENGINE_SRC_DIR}/System/Cursor.cpp
${BANG_ENGINE_SRC_DIR}/System/Library.cpp
${BANG_ENGINE_SRC_DIR}/System/Mutex.cpp
${BANG_ENGINE_SRC_DIR}/System/MutexLocker.cpp
${BANG_ENGINE_SRC_DIR}/System/SystemProcess.cpp
${BANG_ENGINE_SRC_DIR}/System/SystemClipboard.cpp
${BANG_ENGINE_SRC_DIR}/System/SystemUtils.cpp
${BANG_ENGINE_SRC_DIR}/System/Thread.cpp
${BANG_ENGINE_SRC_DIR}/System/ThreadPool.cpp
${BANG_ENGINE_SRC_DIR}/Text/TextFormatter.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/IFocusable.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/ILayoutController.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/ILayoutElement.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIAutoFocuser.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIButton.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UICanvas.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UICheckBox.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIComboBox.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIContentSizeFitter.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIAspectRatioFitter.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIDirLayout.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIFileList.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIFocusable.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIFocusable.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIGridLayout.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIGroupLayout.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIHorizontalLayout.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIImageRenderer.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIInputText.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIInputNumber.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UILayoutIgnorer.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UILayoutElement.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UILayoutManager.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UILabel.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIList.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIMask.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIRectMask.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIRenderer.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIRendererCacher.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIScrollArea.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIScrollBar.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIScrollPanel.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UISlider.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UITextCursor.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UITextRenderer.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UITree.cpp
${BANG_ENGINE_SRC_DIR}/UIComponents/UIVerticalLayout.cpp
${BANG_ENGINE_SRC_DIR}/Windows/Dialog.cpp
${BANG_ENGINE_SRC_DIR}/Windows/DialogWindow.cpp
${BANG_ENGINE_SRC_DIR}/Windows/Window.cpp
${BANG_ENGINE_SRC_DIR}/Windows/WindowManager.cpp
${OTHER_FILES}
)
add_library(BangEngineObjects OBJECT ${ENGINE_SRC_FILES})

list(APPEND ALL_SRC_FILES ${CORE_SRC_FILES})
list(APPEND ALL_SRC_FILES ${GRAPHICS_SRC_FILES})
list(APPEND ALL_SRC_FILES ${ENGINE_SRC_FILES})
add_library(BangStaticLib STATIC $<TARGET_OBJECTS:BangEngineObjects>)
add_library(BangSharedLib SHARED $<TARGET_OBJECTS:BangEngineObjects>)
set_target_properties(BangSharedLib PROPERTIES OUTPUT_NAME Bang)
