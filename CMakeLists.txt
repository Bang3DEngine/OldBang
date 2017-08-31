# CMake Options
cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE off)

project(Bang C CXX)

set(ROOT ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${ROOT}/src)
set(ENGINE_Resources_DIR ${ROOT}/res/EngineResources)
set(ENGINE_SRC_DIR ${SRC_DIR}/Engine)

set(CMAKE_BINARY_DIR ${ROOT}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wl,-O0 -g")
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

# Freetype Dependency
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
link_libraries(${FREETYPE_LIBRARIES})

# SDL Dependency
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARY}" SDL2_LIBRARY)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
link_libraries(${SDL2_LIBRARY})
link_libraries(${SDL2_LIBRARIES})

# OpenAL Dependency
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIRS})
link_libraries(${OPENAL_LIBRARY})

# BangCore Library
include_directories(${ROOT}/include)
include_directories(${ROOT}/include/Bang)
include_directories(${ROOT}/include/freetype)
include_directories(${ROOT}/include/Bang/BangCore)
set(CORE_SRC_FILES
${ENGINE_SRC_DIR}/DataStructures/Array.h
${ENGINE_SRC_DIR}/DataStructures/Flags.h
${ENGINE_SRC_DIR}/DataStructures/List.h
${ENGINE_SRC_DIR}/DataStructures/Map.h
${ENGINE_SRC_DIR}/DataStructures/Set.h
${ENGINE_SRC_DIR}/DataStructures/String.cpp
${ENGINE_SRC_DIR}/DataStructures/TypeMap.h
${ENGINE_SRC_DIR}/Debug/Debug.cpp
${ENGINE_SRC_DIR}/Debug/StreamOperators.cpp
${ENGINE_SRC_DIR}/IO/Files/File.cpp
${ENGINE_SRC_DIR}/IO/Files/Path.cpp
${ENGINE_SRC_DIR}/Math/Color.cpp
${ENGINE_SRC_DIR}/Math/Math.cpp
${ENGINE_SRC_DIR}/Math/Random.cpp
${ENGINE_SRC_DIR}/System/Mutex.cpp
${ENGINE_SRC_DIR}/System/MutexLocker.cpp
${ENGINE_SRC_DIR}/System/SystemProcess.cpp
${ENGINE_SRC_DIR}/System/Thread.cpp
${ENGINE_SRC_DIR}/System/ThreadPool.cpp
)
add_library(BangCoreObjects OBJECT ${CORE_SRC_FILES})
add_library(BangCoreStatic  STATIC $<TARGET_OBJECTS:BangCoreObjects>)
add_library(BangCore        SHARED $<TARGET_OBJECTS:BangCoreObjects>)

# BangGraphics Library
include_directories(${ROOT}/include/Bang/BangGraphics)
set(GRAPHICS_SRC_FILES
${ENGINE_SRC_DIR}/Graphics/Buffers/G_GBuffer.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Image.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/G_Framebuffer.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Font.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_FontSheetCreator.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Material.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_RenderTexture.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Screen.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/G_Shader.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/G_ShaderProgram.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Texture.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_Texture2D.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_TextureUnitManager.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_VAO.cpp
${ENGINE_SRC_DIR}/Graphics/General/G_VBO.cpp
${ENGINE_SRC_DIR}/Graphics/General/GL.cpp
${ENGINE_SRC_DIR}/Graphics/General/GLObject.cpp
${ENGINE_SRC_DIR}/Graphics/General/ImageIO.cpp
)
add_library(BangGraphicsObjects OBJECT ${GRAPHICS_SRC_FILES})
add_library(BangGraphicsStatic  STATIC $<TARGET_OBJECTS:BangGraphicsObjects>)
add_library(BangGraphics        SHARED $<TARGET_OBJECTS:BangGraphicsObjects>)

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
file(GLOB_RECURSE OTHER_FILES "[^.]*.frag" "[^.]*.frag_g" "[^.]*.frag_pp"
                               "[^.]*.vert" "[^.]*.vert_g" "[^.]*.vert_pp"
                               "[^.]*.png" "[^.]*.svg" "[^.]*.bmat")
set(OTHER_FILES
    README.md
    "${OTHER_FILES}"
    )
add_info_files(${OTHER_FILES})

# BangEngine Library
include_directories(${ROOT}/include/Bang/BangEngine)
include_directories(${ROOT}/include/Bang/BangPreprocessor)
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
${ENGINE_SRC_DIR}/Components/Camera.cpp
${ENGINE_SRC_DIR}/Components/CircleRenderer.cpp
${ENGINE_SRC_DIR}/Components/Component.cpp
${ENGINE_SRC_DIR}/Components/DirectionalLight.cpp
${ENGINE_SRC_DIR}/Components/Light.cpp
${ENGINE_SRC_DIR}/Components/LineRenderer.cpp
${ENGINE_SRC_DIR}/Components/MeshRenderer.cpp
${ENGINE_SRC_DIR}/Components/PointLight.cpp
${ENGINE_SRC_DIR}/Components/PostProcessEffect.cpp
${ENGINE_SRC_DIR}/Components/RectTransform.cpp
${ENGINE_SRC_DIR}/Components/Renderer.cpp
${ENGINE_SRC_DIR}/Components/SingleLineRenderer.cpp
${ENGINE_SRC_DIR}/Components/Transform.cpp
${ENGINE_SRC_DIR}/Debug/Chrono.cpp
${ENGINE_SRC_DIR}/Debug/ChronoGL.cpp
${ENGINE_SRC_DIR}/Debug/GraphicPipelineDebugger.cpp
${ENGINE_SRC_DIR}/Factory/ComponentFactory.cpp
${ENGINE_SRC_DIR}/Factory/GameObjectFactory.cpp
${ENGINE_SRC_DIR}/Factory/MaterialFactory.cpp
${ENGINE_SRC_DIR}/Factory/MeshFactory.cpp
${ENGINE_SRC_DIR}/GameBuilder/GameBuilder.cpp
${ENGINE_SRC_DIR}/GameObjects/GameObject.cpp
${ENGINE_SRC_DIR}/GameObjects/Scene.cpp
${ENGINE_SRC_DIR}/GameObjects/UIGameObject.cpp
${ENGINE_SRC_DIR}/General/Application.cpp
${ENGINE_SRC_DIR}/General/ICloneable.cpp
${ENGINE_SRC_DIR}/General/Input.cpp
${ENGINE_SRC_DIR}/General/SceneManager.cpp
${ENGINE_SRC_DIR}/General/Object.cpp
${ENGINE_SRC_DIR}/General/Selection.cpp
${ENGINE_SRC_DIR}/General/Time.cpp
${ENGINE_SRC_DIR}/Gizmos/Gizmos.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/SelectionFramebuffer.cpp
${ENGINE_SRC_DIR}/Graphics/General/Screen.cpp
${ENGINE_SRC_DIR}/Graphics/GraphicPipeline/GraphicPipeline.cpp
${ENGINE_SRC_DIR}/Graphics/ShaderManager.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/Shader.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/ShaderPreprocessor.cpp
${ENGINE_SRC_DIR}/ImportFiles/GUID.h
${ENGINE_SRC_DIR}/ImportFiles/GUIDManager.cpp
${ENGINE_SRC_DIR}/ImportFiles/ImportFilesManager.cpp
${ENGINE_SRC_DIR}/IO/Behaviours/BehaviourManager.cpp
${ENGINE_SRC_DIR}/IO/General/Paths.cpp
${ENGINE_SRC_DIR}/IO/Files/Extensions.cpp
${ENGINE_SRC_DIR}/IO/Files/MeshIO.cpp
${ENGINE_SRC_DIR}/IO/Project/Project.cpp
${ENGINE_SRC_DIR}/IO/Project/ProjectManager.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLAttribute.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLNode.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLParser.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLProperty.cpp
${ENGINE_SRC_DIR}/Math/AABox.cpp
${ENGINE_SRC_DIR}/Math/Sphere.cpp
${ENGINE_SRC_DIR}/Resources/Asset.cpp
${ENGINE_SRC_DIR}/Resources/AudioClip.cpp
${ENGINE_SRC_DIR}/Resources/Font.cpp
${ENGINE_SRC_DIR}/Resources/Material.cpp
${ENGINE_SRC_DIR}/Resources/Mesh.cpp
${ENGINE_SRC_DIR}/Resources/Prefab.cpp
${ENGINE_SRC_DIR}/Resources/Resource.cpp
${ENGINE_SRC_DIR}/Resources/Resources.cpp
${ENGINE_SRC_DIR}/Resources/Serializable.cpp
${ENGINE_SRC_DIR}/Resources/ShaderProgram.cpp
${ENGINE_SRC_DIR}/Resources/Texture2D.cpp
${ENGINE_SRC_DIR}/System/Library.cpp
${ENGINE_SRC_DIR}/System/SystemUtils.cpp
${ENGINE_SRC_DIR}/Text/TextFormatter.cpp
${ENGINE_SRC_DIR}/UIComponents/UIButton.cpp
${ENGINE_SRC_DIR}/UIComponents/UIBorderRect.cpp
${ENGINE_SRC_DIR}/UIComponents/UIButtonTinter.cpp
${ENGINE_SRC_DIR}/UIComponents/UICanvas.cpp
${ENGINE_SRC_DIR}/UIComponents/UIDirLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIHorizontalLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/UIImageRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UIInputText.cpp
${ENGINE_SRC_DIR}/UIComponents/UIMask.cpp
${ENGINE_SRC_DIR}/UIComponents/UIRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UIScrollArea.cpp
${ENGINE_SRC_DIR}/UIComponents/UITextCursor.cpp
${ENGINE_SRC_DIR}/UIComponents/UITextRenderer.cpp
${ENGINE_SRC_DIR}/UIComponents/UIVerticalLayout.cpp
${ENGINE_SRC_DIR}/UIComponents/IFocusListener.h
${ENGINE_SRC_DIR}/Windows/Window.cpp
${OTHER_FILES}
)
add_library(BangEngineObjects OBJECT ${ENGINE_SRC_FILES})
add_library(BangEngineStatic  STATIC $<TARGET_OBJECTS:BangEngineObjects>)
add_library(BangEngine        SHARED $<TARGET_OBJECTS:BangEngineObjects>)

list(APPEND ALL_SRC_FILES ${CORE_SRC_FILES})
list(APPEND ALL_SRC_FILES ${GRAPHICS_SRC_FILES})
list(APPEND ALL_SRC_FILES ${ENGINE_SRC_FILES})
add_library(BangStatic STATIC $<TARGET_OBJECTS:BangCoreObjects>
                              $<TARGET_OBJECTS:BangGraphicsObjects>
                              $<TARGET_OBJECTS:BangEngineObjects>)
add_library(BangShared SHARED $<TARGET_OBJECTS:BangCoreObjects>
                              $<TARGET_OBJECTS:BangGraphicsObjects>
                              $<TARGET_OBJECTS:BangEngineObjects>)
set_target_properties(BangShared PROPERTIES OUTPUT_NAME Bang)


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
add_executable(BangMain ${SRC_DIR}/Main.cpp)
add_executable(Game.exe ${SRC_DIR}/MainGame.cpp)
add_executable(BangGameBuilder ${SRC_DIR}/MainGameBuilder.cpp)
add_dependencies(BangPreprocessor BangShared)
add_dependencies(BangMain BangShared)
add_dependencies(Game.exe BangShared)
add_dependencies(BangGameBuilder BangShared)
