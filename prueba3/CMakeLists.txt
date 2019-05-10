cmake_minimum_required (VERSION 2.6)
project (ecc)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -std=c++14" )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/tests)

add_executable(${PROJECT_NAME} main.cpp grammar.cpp LL1.cpp)

# add_executable(test_grammar_loader ${PROJECT_SOURCE_DIR}/tests/test_grammar_loader.cpp
#                                    LL1.cpp
#                                   grammar.cpp)
                                    
