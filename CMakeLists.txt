cmake_minimum_required(VERSION 3.10)

# Project name and version
project(a VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/include)

# Source files
file(GLOB_RECURSE PROJECT_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/external/src/*.cpp
  ${CMAKE_SOURCE_DIR}/external/src/*.c
)


# ============================== glfw ============================== 
# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glfw)

# Suppress all warnings for GLFW
target_compile_options(glfw PRIVATE
    -w  # Suppress all warnings
)
# ============================================================ 
# ============================== stb_image ============================== 
# ============================================================ 
# ============================== opengl ============================== 
find_package(OpenGL REQUIRED)
# ============================================================ 

# Create executable
add_executable(main ${PROJECT_SOURCES})

target_link_libraries(main PRIVATE  glfw  OpenGL::GL)
target_compile_options(main PRIVATE -Wall)
