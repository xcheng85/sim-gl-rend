cmake_minimum_required(VERSION 3.29)
project(sim-gl-rend LANGUAGES CUDA CXX VERSION 1.0.0 )
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#include cmake script
include(CHECK_INSOURCE_BUILD)
# include(BOOST)
# include(CUDA)

# c++ version

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
if(NOT glfw_POPULATED)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Populate(glfw)
endif()

add_subdirectory(${glfw_SOURCE_DIR})

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_Populate(stb)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG        VER-2-13-0
)
FetchContent_GetProperties(freetype)
if(NOT freetype_POPULATED)
        FetchContent_Populate(freetype)
        # Set variables for FreeType
        set(FT_DISABLE_ZLIB ON CACHE BOOL "Disable zlib for embedded Freetype" FORCE)
        set(FT_DISABLE_BZIP2 ON CACHE BOOL "Disable bzip2 for embedded Freetype" FORCE)
        set(FT_DISABLE_PNG ON CACHE BOOL "Disable libpng for embedded Freetype" FORCE)
        set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable harfbuzz for embedded Freetype" FORCE)
        set(FT_DISABLE_BROTLI ON CACHE BOOL "Disable brotli for embedded Freetype" FORCE)
        set(BUILD_SHARED_LIBS OFF)       # Force static link

        # Import FreeType directory
        add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if (NOT ANDROID)
  include_directories(${GLFW_SOURCE_DIR}/include)
endif()
include_directories(${freetype_SOURCE_DIR}/include)

# for experimental/future
#GCC provides experimental support for the upcoming ISO C++ standard, C++0x. 
#This support can be enabled with the -std=c++0x or -std=gnu++0x compiler options; 
#the former disables GNU extensions.
# set(CMAKE_CXX_FLAGS "-std=c++0x -lstdc++fs")

add_subdirectory(src bin)
add_subdirectory(glad)
