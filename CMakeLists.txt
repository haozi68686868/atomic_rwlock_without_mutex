cmake_minimum_required(VERSION 2.8.12)
PROJECT (multiThread_Test)
SET(CMAKE_BUILD_TYPE "Debug")

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

add_definitions(-Wreturn-type)
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -pthread")

file(GLOB_RECURSE SRC_LIST "src/*.cpp")

## LIB_SUFFIX
if(NOT DEFINED TARGET_OS)
    if(CMAKE_HOST_SYSTEM_NAME MATCHES Linux*)
        set(TARGET_OS linux)
    elseif(CMAKE_HOST_SYSTEM_NAME MATCHES Darwin*)
        set(TARGET_OS mac)
    elseif(CMAKE_HOST_SYSTEM_NAME MATCHES Windows*)
        set(TARGET_OS windows)
    endif()
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES i.86 OR BUILD_I386)
    set(PLATFORM_NAME x86)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES x86*)
    set(PLATFORM_NAME x86_64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES armv7*)
    set(PLATFORM_NAME armv7)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES aarch64)
    set(PLATFORM_NAME aarch64)
endif()

message(STATUS "Build platform name is :" ${TARGET_OS}_${PLATFORM_NAME})
message(STATUS "Build target os is : ${TARGET_OS}")

#find_package(Boost REQUIRED filesystem)
#message(STATUS "Boost Include DIR is : ${Boost_INCLUDE_DIR}")
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${Boost_INCLUDE_DIR})
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST} )
# target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} atomic )



