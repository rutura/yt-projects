cmake_minimum_required(VERSION 3.25)

project(QtCreatorDebuggingLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to a Debug build so the first CMake configure a student does in
# Qt Creator already produces a fully debuggable binary (unoptimized, with
# debug symbols) via CMake's normal, compiler-agnostic Debug config.
# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
# endif()

add_executable(qt_creator_debugging_lab
    src/main.cpp
    src/01_breakpoints_stepping.cpp
    src/02_variables_watches.cpp
    src/03_call_stack_recursion.cpp
    src/04_conditional_breakpoints.cpp
    src/05_pointers_memory.cpp
    src/06_containers_stl.cpp
    src/07_exceptions.cpp
    src/08_templates_generic.cpp
    src/09_multithreading.cpp
    src/scenarios.h
)

target_include_directories(qt_creator_debugging_lab PRIVATE src)

find_package(Threads REQUIRED)
target_link_libraries(qt_creator_debugging_lab PRIVATE Threads::Threads)
