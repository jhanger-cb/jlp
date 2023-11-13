cmake_minimum_required(VERSION 3.27.7)

set(prjName jlp)

project(${prjName}
  VERSION 0.3.2
  DESCRIPTION "Java Log Parser")


### Future Cross Compilability (*_COINED_*);
# if(WIN32)
#   do some things; 
# elseif(MACOS)
#   do some other things;
# else() #linux;
#   do the guru things;
# endif()

### Common Settings to all Relases:
set (JLP_SRCS main.cpp javaLogParser.cpp javaLogEntry.cpp javaStackTrace.cpp)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

## BEFORE configure_file(); 
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  set(PROJECT_VERSION_RELEASE "-debug")
  message("Debug Set")
else()
  set(PROJECT_VERSION_RELEASE "-stable")
  message("Debug NOT Set")
endif()

configure_file("config.h.in" "config.h")
include_directories(${PROJECT_BINARY_BIN}) 

#### Set Release Specific Settings:
if(NOT CMAKE_BUILD_TYPE) 
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

add_compile_options(
  -Wall
  -Wextra
  $<$<CONFIG:DEBUG>:-g>
  $<$<CONFIG:DEBUG>:-pg>
  $<$<CONFIG:RELEASE>:-O3>
)

add_compile_definitions(
  $<$<CONFIG:DEBUG>:DEBUG>
)


add_executable(jlp ${JLP_SRCS})
find_package(Boost 1.74.0 COMPONENTS program_options) 
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS}) 
  target_link_libraries(jlp ${Boost_LIBRARIES})
endif()