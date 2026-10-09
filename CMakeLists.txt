##### ANORRL CMake for Linux Systems #####
# Hi so, this is based off projectpizza's cmake, but modified to be unix only, and other anorrl things
# uhm yeah

cmake_minimum_required(VERSION 3.20)

project(anorrl C CXX ASM)

add_compile_definitions(BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(_DEBUG)
endif()

string(TIMESTAMP BUILD_TIME "%s")
string(RANDOM LENGTH 8 ALPHABET 0123456789 RANDOM_SEED ${BUILD_TIME} RANDOM_VALUE)
math(EXPR ARL_BUILDSEED "${RANDOM_VALUE} % 1000000")
set(RANDOM_CONSTANT_H "${CMAKE_SOURCE_DIR}/App/include/security/RandomConstant.h")
if(NOT EXISTS "${RANDOM_CONSTANT_H}")
  file(WRITE "${RANDOM_CONSTANT_H}" "#pragma once\n#define ARL_BUILDSEED ${ARL_BUILDSEED}\n")
endif()
include_directories(${CMAKE_SOURCE_DIR}/App/include/security)
include_directories("${CMAKE_SOURCE_DIR}/Rendering/RbxG3D/include")

option(ACC_BUILD "Build in ACCService mode" OFF)
if(ACC_BUILD)
  message(STATUS "Building for ANORRLCloudCompute ...")
  add_compile_definitions(ARL_ACC_SECURITY)
endif()

option(STUDIO_BUILD "Build in Studio mode" OFF)
if(STUDIO_BUILD)
  message(STATUS "Building for ANORRLStudio ...")
  add_compile_definitions(ARL_STUDIO_BUILD)
endif()

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-deprecated-declarations)
  add_compile_options(-Wno-nonportable-include-path)
  add_compile_options(-Wno-ignored-attributes)
  add_compile_options(-Wno-undefined-var-template)
  add_compile_options(-Wno-extra-tokens)
  add_compile_options(-Wno-unused-command-line-argument)
  add_compile_options(-Wno-delete-abstract-non-virtual-dtor)
  add_compile_options(-Wno-implicit-const-int-float-conversion)
  add_compile_options(-Wno-return-stack-address)
  add_compile_options(-Wno-nontrivial-memaccess)
  add_compile_options(-Wno-string-plus-int)
  add_compile_options(-Wno-format)
  add_compile_options(-Wno-constant-logical-operand)
endif()

include_directories(${CMAKE_SOURCE_DIR}/fmod/include)
set(FMOD_ARCHITECTURE x64)
link_directories(${CMAKE_SOURCE_DIR}/fmod/lib/linux/${FMOD_ARCHITECTURE})
set(RUNTIME_FILES ${CMAKE_SOURCE_DIR}/fmod/lib/linux/${FMOD_ARCHITECTURE}/libfmod.so.13)

add_library(fmod SHARED IMPORTED)
set_property(TARGET fmod PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/fmod/lib/linux/${FMOD_ARCHITECTURE}/libfmod.so)

if(LINUX)
  find_package(bgfx CONFIG REQUIRED)
endif()
find_package(Boost REQUIRED COMPONENTS random format accumulators bimap filesystem system iostreams thread exception program_options pool circular_buffer math crc numeric_conversion flyweight numeric_ublas foreach REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(glad REQUIRED)

if(STUDIO_BUILD)
  set(Qt5_DIR "/usr/lib/cmake/Qt5")
  set(Qt5Core_DIR "/usr/lib/cmake/Qt5Core")
  set(Qt5Gui_DIR "/usr/lib/cmake/Qt5Gui")
  set(Qt5Widgets_DIR "/usr/lib/cmake/Qt5Widgets")
  set(Qt5Concurrent_DIR "/usr/lib/cmake/Qt5Concurrent")
  set(Qt5Network_DIR "/usr/lib/cmake/Qt5Network")
  set(Qt5Xml_DIR "/usr/lib/cmake/Qt5Xml")
  set(Qt5WebKit_DIR "/usr/lib/cmake/Qt5WebKit")
  set(Qt5WebKitWidgets_DIR "/usr/lib/cmake/Qt5WebKitWidgets")
  set(Qt5WebChannel_DIR "/usr/lib/cmake/Qt5WebChannel")
  set(Qt5Positioning_DIR "/usr/lib/cmake/Qt5Positioning")
  set(Qt5OpenGL_DIR "/usr/lib/cmake/Qt5OpenGL")
  
  find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Xml
    WebKit
    WebKitWidgets
    WebChannel
    Positioning
    Concurrent
	OpenGL
  )
endif()

if(NOT ACC_BUILD)
  find_package(SDL2 CONFIG REQUIRED)
endif()

if(NOT STUDIO_BUILD)
  add_subdirectory(CoreScriptConverter2)
endif()

find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)

include_directories(${Boost_INCLUDE_DIRS})
include(ListFilter)
include(IncludeProjectFiles)

if(NOT ARL_SETTINGS_INITIALIZED)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(ARL_SETTINGS_INITIALIZED TRUE CACHE INTERNAL "" FORCE)
endif(NOT ARL_SETTINGS_INITIALIZED)

add_subdirectory(App)
add_subdirectory(App.BulletPhysics)
add_subdirectory(Base)
add_subdirectory(Log)
add_subdirectory(Rendering)
add_subdirectory(Network)
add_subdirectory(ClientShared)

if(ACC_BUILD)
  add_subdirectory(ANORRLCloudCompute)
elseif(STUDIO_BUILD)
  add_subdirectory(CSG)
  add_subdirectory(QTitanRibbon)
  add_subdirectory(ANORRLStudio)
else()
  add_subdirectory(ANORRLUnixClient)
endif()

set(ARL_LINKER_DEPENDENCY_FLAGS "-Wl,--no-whole-archive,--no-gc-sections,--no-c++11-narrowing")
