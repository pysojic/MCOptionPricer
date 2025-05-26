cmake_minimum_required(VERSION 3.14)
project(MonteCarloPricer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-14")
#set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

# Specify the source files
set(SOURCES
    src/main.cpp
    src/Interface.cpp
    src/PricerDerived.cpp
    src/RNGDerived.cpp
    src/SDEConcrete.cpp
    #src/ThreadPool.cpp
)

add_executable(MonteCarloPricer ${SOURCES})

# Add warnings for GCC/Clang
target_compile_options(MonteCarloPricer PRIVATE -O0 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic

target_include_directories(MonteCarloPricer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)