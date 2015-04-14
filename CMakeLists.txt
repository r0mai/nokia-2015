cmake_minimum_required(VERSION 2.8)

project(deadline24-2015)

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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost COMPONENTS
    regex
    system
    program_options
    thread)

find_package(GMP)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${GMP_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O2")

subdirlist(SUBDIRS ${CMAKE_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
    if(NOT SUBDIR STREQUAL ".git" AND
        NOT SUBDIR STREQUAL "build" AND
        NOT SUBDIR STREQUAL "cmake")
        set(SOURCES "")
        aux_source_directory(${SUBDIR} SOURCES)
        set(EXEC_NAME ${SUBDIR})
        add_executable(${EXEC_NAME} ${SOURCES})
        target_link_libraries(${EXEC_NAME} ${Boost_LIBRARIES})
        target_link_libraries(${EXEC_NAME} ${GMP_LIBRARIES})
    endif()
endforeach()
