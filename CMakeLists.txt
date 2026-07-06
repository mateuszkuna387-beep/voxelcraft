cmake_minimum_required(VERSION 3.20)
project(VoxelCraft VERSION 1.0.0 LANGUAGES CXX C)

# Workaround for CMake 4.3.4 bug on MSYS2 (missing internal C variables)
if(MINGW AND CMAKE_VERSION VERSION_GREATER_EQUAL "4.3")
  set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>" CACHE INTERNAL "" FORCE)
  set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>" CACHE INTERNAL "" FORCE)
  set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>" CACHE INTERNAL "" FORCE)
  set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> cr <TARGET> <OBJECTS>" CACHE INTERNAL "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---- GLAD (OpenGL loader) ----
add_library(glad STATIC src/third_party/glad/gl.c)
target_include_directories(glad PUBLIC src/third_party/glad/include)

# ---- Dependencies ----
include(FetchContent)

# GLM (OpenGL Mathematics)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLFW (windowing)
find_package(glfw3 REQUIRED)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---- Subdirectories ----
add_subdirectory(src)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
