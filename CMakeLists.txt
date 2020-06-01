cmake_minimum_required(VERSION 3.1.0)

project (main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -g -pthread -pg") 

if ((DEFINED ENV{USE_SANITIZER}) AND ($ENV{USE_SANITIZER} STREQUAL "YES")) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls") 
  add_definitions(-D_GLIBCXXDEBUG -DGLIBCXXDEBUG_PEDANTIC)
endif()

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB MAIN_SRC "*.cpp")
file(COPY "./test" DESTINATION ${CMAKE_BINARY_DIR})

add_executable(main ${MAIN_SRC})

set_property(TARGET main PROPERTY CXX_STANDARD 17)
