cmake_minimum_required(VERSION 3.20.0)
project(jlp 
  VERSION 0.3.0 
  DESCRIPTION "Java Log Parser")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

configure_file("config.h.in" "config.h")
include_directories(${PROJECT_BINARY_BIN}) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.74.0 COMPONENTS program_options) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(jlp main.cpp javaLogParser.cpp javaLogEntry.cpp javaStackTrace.cpp)
    target_link_libraries(jlp ${Boost_LIBRARIES})
endif()
