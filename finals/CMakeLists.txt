cmake_minimum_required(VERSION 2.8)

project(sample-cxx)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost COMPONENTS
    regex
    system
    program_options
    thread)

find_package(SFML 2 REQUIRED
    system
    window
    graphics
    network
    audio)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${SFML_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2")

aux_source_directory(src SRC_SOURCES)
aux_source_directory(bin BIN_SOURCES)

set(SRC_LIB_NAME ${CMAKE_PROJECT_NAME}-src)
set(BIN_EXEC_NAME ${CMAKE_PROJECT_NAME}-bin)

add_library(${SRC_LIB_NAME} STATIC ${SRC_SOURCES})
target_link_libraries(${SRC_LIB_NAME} ${Boost_LIBRARIES})
target_link_libraries(${SRC_LIB_NAME} ${SFML_LIBRARIES})

add_executable(${BIN_EXEC_NAME} ${BIN_SOURCES})
target_link_libraries(${BIN_EXEC_NAME} ${SRC_LIB_NAME})

include_directories(src)
