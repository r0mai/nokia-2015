cmake_minimum_required(VERSION 2.8)

project(finals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

macro(AUX_RECURSIVE_SOURCE_DIRECTORY directory result)
    aux_source_directory(${directory} DIR_SOURCES)
    subdirlist(DIR_SUBDIRS "${CMAKE_SOURCE_DIR}/${directory}")
    foreach(DIR_SUBDIR ${DIR_SUBDIRS})
        set(DIR_SOURCES_TMP "")
        aux_source_directory(
            "${CMAKE_SOURCE_DIR}/${directory}/${DIR_SUBDIR}" DIR_SOURCES_TMP)
        list(APPEND DIR_SOURCES ${DIR_SOURCES_TMP})
    endforeach()
    set(${result} ${DIR_SOURCES})
endmacro()

find_package(Boost)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2.3 REQUIRED
    system
    window
    graphics)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${SFML_INCLUDE_DIR})

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2")
endif()

aux_recursive_source_directory(src SRC_SOURCES)
aux_recursive_source_directory(test TEST_SOURCES)

aux_source_directory(bin BIN_SOURCES)

set(TEST_EXEC_NAME test-${CMAKE_PROJECT_NAME})
set(SRC_LIB_NAME ${CMAKE_PROJECT_NAME}-src)
set(BIN_EXEC_NAME ${CMAKE_PROJECT_NAME}-bin)

add_library(${SRC_LIB_NAME} STATIC ${SRC_SOURCES} ${SRC_AGENT_SOURCES})
target_link_libraries(${SRC_LIB_NAME} ${Boost_LIBRARIES})
target_link_libraries(${SRC_LIB_NAME} ${SFML_LIBRARIES})
target_link_libraries(${SRC_LIB_NAME} ${SFML_DEPENDENCIES})

add_executable(${BIN_EXEC_NAME} ${BIN_SOURCES})
target_link_libraries(${BIN_EXEC_NAME} ${SRC_LIB_NAME})

add_executable(${TEST_EXEC_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_EXEC_NAME} ${SRC_LIB_NAME})

include_directories(src)
