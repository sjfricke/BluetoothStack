cmake_minimum_required(VERSION 3.10.0)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(COMMON_DIR ${ROOT_DIR}/common)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

project(common)
include_directories(${COMMON_DIR})
file(GLOB COMMON_SRC ${COMMON_DIR}/*.cpp)
add_library(${CMAKE_PROJECT_NAME} STATIC ${COMMON_SRC})

project(client_classic)
include_directories(${ROOT_DIR})
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/client/bluetooth_classic_client.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} common bluetooth)

project(server_classic)
include_directories(${ROOT_DIR})
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/server/bluetooth_classic_server.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} common bluetooth)
