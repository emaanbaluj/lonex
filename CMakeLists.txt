cmake_minimum_required(VERSION 3.10)
project(lonex)  # Set the project name to "lonex"

# Manually define Boost paths
set(Boost_INCLUDE_DIR /opt/homebrew/opt/boost/include)
set(Boost_LIBRARY_DIR /opt/homebrew/opt/boost/lib)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# Ensure Boost is found
message(STATUS "Boost Include Dir: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost Library Dir: ${Boost_LIBRARY_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lonex main.cpp)
target_link_libraries(lonex ${Boost_LIBRARY_DIR}/libboost_system.a)