cmake_minimum_required (VERSION 3.0)
project (test_ll1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../)
  
add_executable(${PROJECT_NAME}  test_ll1.cpp
                                ${PROJECT_SOURCE_DIR}/../grammar.cpp
                                ${PROJECT_SOURCE_DIR}/../LL1.cpp)
