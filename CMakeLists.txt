cmake_minimum_required(VERSION 3.16)
project(lr-parser LANGUAGES CXX)

find_package(FLEX REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GRAPHVIZ REQUIRED libgvc libcgraph)

include_directories(${GRAPHVIZ_INCLUDE_DIRS})

set(PARSER_TARGET lr-parser)
set(LEXER_TARGET lexer)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(LEXER_L ${SRC_DIR}/lexer.l)
set(LEXER_CPP ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

flex_target(${LEXER_TARGET} ${LEXER_L} ${LEXER_CPP})
set_source_files_properties(${LEXER_CPP} PROPERTIES COMPILE_FLAGS "-w")
set_source_files_properties(${LEXER_CPP} PROPERTIES GENERATED TRUE)
set(GENERATED_LEXER_FILE "${LEXER_CPP}" CACHE INTERNAL "")

function(add_exec target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${GRAPHVIZ_INCLUDE_DIRS})
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_link_libraries(${target} PRIVATE fl ${GRAPHVIZ_LIBRARIES})
endfunction()

function(add_warnings target)
    target_compile_options(${target} PRIVATE 
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wold-style-cast
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )
endfunction()

add_exec(${PARSER_TARGET} ${SRC_DIR}/main.cpp ${FLEX_${LEXER_TARGET}_OUTPUTS})


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_warnings(${PARSER_TARGET})
endif()

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    add_subdirectory(${TESTS_DIR})
endif()