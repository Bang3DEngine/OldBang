# CMake Options
cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE off)

project(Bang C CXX)

set(ROOT ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${ROOT}/src)
set(ENGINE_SRC_DIR ${SRC_DIR}/Engine)

set(CMAKE_BINARY_DIR ${ROOT}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wl,-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wl,-O3")
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
message(" =============== Building in ${CMAKE_BUILD_TYPE} mode...")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(PROJECT_SOURCE_DIR ${SRC_DIR})

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
include_directories(${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
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
add_library(
BangCore
${ENGINE_SRC_DIR}/DataStructures/Flags.h
${ENGINE_SRC_DIR}/DataStructures/String.cpp
${ENGINE_SRC_DIR}/Debug/Debug.cpp
${ENGINE_SRC_DIR}/Debug/StreamOperators.cpp
${ENGINE_SRC_DIR}/General/Mutex.cpp
${ENGINE_SRC_DIR}/General/MutexLocker.cpp
${ENGINE_SRC_DIR}/General/SystemProcess.cpp
${ENGINE_SRC_DIR}/General/Thread.cpp
${ENGINE_SRC_DIR}/General/ThreadPool.cpp
${ENGINE_SRC_DIR}/IO/Files/File.cpp
${ENGINE_SRC_DIR}/IO/Files/Path.cpp
${ENGINE_SRC_DIR}/Math/Color.cpp
${ENGINE_SRC_DIR}/Math/Math.cpp
${ENGINE_SRC_DIR}/Math/Random.cpp
)

# BangGraphics Library
include_directories(${ROOT}/include/Bang/BangGraphics)
add_library(
BangGraphics
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

# BangEngine Library
include_directories(${ROOT}/include/Bang/BangEngine)
include_directories(${ROOT}/include/Bang/BangPreprocessor)
add_library(
BangEngine
${ENGINE_SRC_DIR}/Assets/Asset.cpp
${ENGINE_SRC_DIR}/Assets/AudioClip.cpp
${ENGINE_SRC_DIR}/Assets/Font.cpp
${ENGINE_SRC_DIR}/Assets/Material.cpp
${ENGINE_SRC_DIR}/Assets/Mesh.cpp
${ENGINE_SRC_DIR}/Assets/MeshFactory.cpp
${ENGINE_SRC_DIR}/Assets/Prefab.cpp
${ENGINE_SRC_DIR}/Assets/ShaderProgram.cpp
${ENGINE_SRC_DIR}/Assets/Texture2D.cpp
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
${ENGINE_SRC_DIR}/Components/ComponentFactory.cpp
${ENGINE_SRC_DIR}/Components/DirectionalLight.cpp
${ENGINE_SRC_DIR}/Components/UIComponent.cpp
${ENGINE_SRC_DIR}/Components/Light.cpp
${ENGINE_SRC_DIR}/Components/LineRenderer.cpp
${ENGINE_SRC_DIR}/Components/MeshRenderer.cpp
${ENGINE_SRC_DIR}/Components/PointLight.cpp
${ENGINE_SRC_DIR}/Components/PostProcessEffect.cpp
${ENGINE_SRC_DIR}/Components/RectTransform.cpp
${ENGINE_SRC_DIR}/Components/Renderer.cpp
${ENGINE_SRC_DIR}/Components/SingleLineRenderer.cpp
${ENGINE_SRC_DIR}/Components/Transform.cpp
${ENGINE_SRC_DIR}/Components/UIImageRenderer.cpp
${ENGINE_SRC_DIR}/Components/UIButton.cpp
${ENGINE_SRC_DIR}/Components/UIBorderRect.cpp
${ENGINE_SRC_DIR}/Components/UIButtonTinter.cpp
${ENGINE_SRC_DIR}/Components/UIRenderer.cpp
${ENGINE_SRC_DIR}/Components/UITextRenderer.cpp
${ENGINE_SRC_DIR}/GameBuilder/GameBuilder.cpp
${ENGINE_SRC_DIR}/GameObjects/GameObject.cpp
${ENGINE_SRC_DIR}/GameObjects/GameObjectFactory.cpp
${ENGINE_SRC_DIR}/GameObjects/Scene.cpp
${ENGINE_SRC_DIR}/General/Application.cpp
${ENGINE_SRC_DIR}/General/DistanceFielder.cpp
${ENGINE_SRC_DIR}/General/ICloneable.cpp
${ENGINE_SRC_DIR}/General/Input.cpp
${ENGINE_SRC_DIR}/General/Library.cpp
${ENGINE_SRC_DIR}/General/MaterialFactory.cpp
${ENGINE_SRC_DIR}/General/SceneManager.cpp
${ENGINE_SRC_DIR}/General/Selection.cpp
${ENGINE_SRC_DIR}/General/SystemUtils.cpp
${ENGINE_SRC_DIR}/General/TextFormatter.cpp
${ENGINE_SRC_DIR}/General/Time.cpp
${ENGINE_SRC_DIR}/Gizmos/Gizmos.cpp
${ENGINE_SRC_DIR}/Graphics/Buffers/SelectionFramebuffer.cpp
${ENGINE_SRC_DIR}/Graphics/General/Screen.cpp
${ENGINE_SRC_DIR}/Graphics/GraphicPipeline/GraphicPipeline.cpp
${ENGINE_SRC_DIR}/Graphics/ShaderManager.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/Shader.cpp
${ENGINE_SRC_DIR}/Graphics/Shaders/ShaderPreprocessor.cpp
${ENGINE_SRC_DIR}/Debug/Chrono.cpp
${ENGINE_SRC_DIR}/Debug/ChronoGL.cpp
${ENGINE_SRC_DIR}/Debug/GraphicPipelineDebugger.cpp
${ENGINE_SRC_DIR}/ImportFiles/GUID.h
${ENGINE_SRC_DIR}/ImportFiles/GUIDManager.cpp
${ENGINE_SRC_DIR}/ImportFiles/ImportFilesManager.cpp
${ENGINE_SRC_DIR}/IO/Behaviours/BehaviourManager.cpp
${ENGINE_SRC_DIR}/IO/General/Paths.cpp
${ENGINE_SRC_DIR}/IO/General/Resource.cpp
${ENGINE_SRC_DIR}/IO/General/Resources.cpp
${ENGINE_SRC_DIR}/IO/General/SerializableObject.cpp
${ENGINE_SRC_DIR}/IO/Files/Extensions.cpp
${ENGINE_SRC_DIR}/IO/Files/FileReader.cpp
${ENGINE_SRC_DIR}/IO/Project/Project.cpp
${ENGINE_SRC_DIR}/IO/Project/ProjectManager.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLAttribute.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLNode.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLParser.cpp
${ENGINE_SRC_DIR}/IO/XML/XMLProperty.cpp
${ENGINE_SRC_DIR}/Math/AABox.cpp
${ENGINE_SRC_DIR}/Math/Sphere.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIButton.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUICanvas.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIDirLayout.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIHorizontalLayout.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIInputText.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIMask.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIScrollArea.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUITextCursor.cpp
${ENGINE_SRC_DIR}/UIGameObjects/GUIVerticalLayout.cpp
${ENGINE_SRC_DIR}/UIGameObjects/IFocusable.h
${ENGINE_SRC_DIR}/UIGameObjects/UIGameObject.cpp
${ENGINE_SRC_DIR}/Windows/Window.cpp
)

# BangPreprocessor Executable
add_executable(
BangPreprocessor
${SRC_DIR}/MainBangPreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BangPreprocessor.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedStruct.cpp
${ENGINE_SRC_DIR}/Compiler/BangPreprocessor/BPReflectedVariable.cpp
)
target_link_libraries(BangPreprocessor
BangCore
)

# BangMain Executable
add_executable(
Bang
${SRC_DIR}/Main.cpp
)
target_link_libraries(Bang
-Wl,--start-group BangEngine BangGraphics BangCore -Wl,--end-group
)

# BangMainGame Executable
add_executable(
Game.exe
${SRC_DIR}/MainGame.cpp
)
target_link_libraries(Game.exe
-Wl,--start-group BangEngine BangGraphics BangCore -Wl,--end-group
)

# BangMainGameBuilder Executable
add_executable(
BangGameBuilder
${SRC_DIR}/MainGameBuilder.cpp
)
target_link_libraries(BangGameBuilder
-Wl,--start-group BangEngine BangGraphics BangCore -Wl,--end-group
)
