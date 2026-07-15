cmake_minimum_required(VERSION 3.25)

project(CppDebuggingLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp_debugging_lab
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

target_include_directories(cpp_debugging_lab PRIVATE src)

find_package(Threads REQUIRED)
target_link_libraries(cpp_debugging_lab PRIVATE Threads::Threads)
