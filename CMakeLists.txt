cmake_minimum_required(VERSION 3.1.0)

project (main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -pg") 

if (($ENV{MODE} STREQUAL "DEBUG")) 
  message(" ** Building in debug mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fno-optimize-sibling-calls") 
  add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC _LIBCPP_DEBUG=1 RUN_TESTS)
elseif (($ENV{MODE} STREQUAL "TEST"))  
  message(" ** Building in test mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 
  add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC _LIBCPP_DEBUG=1 RUN_TESTS)
else()
  message(" ** Building in release mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 
endif()

file(GLOB MAIN_SRC "*.cpp")
file(COPY "./test" DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR})
add_executable(main ${MAIN_SRC})

set_property(TARGET main PROPERTY CXX_STANDARD 17)
