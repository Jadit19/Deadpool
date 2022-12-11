cmake_minimum_required(VERSION 3.16)
project(client VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)

find_package(Boost 1.80.0 REQUIRED COMPONENTS system thread)

add_executable(${PROJECT_NAME}
  ./src/main.cpp
  ./src/tcp_client.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    critical
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${Boost_LIBRARIES}
    critical
)