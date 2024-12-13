cmake_minimum_required(VERSION 3.10)
project(Tools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

file(GLOB_RECURSE HEADERS
    include/*.h
)

# Define groups (filters)
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})

# Create the lib
add_library(Tools STATIC ${SOURCES} ${HEADERS})

# Specify the include directory for this library
target_include_directories(Tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories
target_include_directories(Tools PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add precompiled headers if needed
# target_precompile_headers(Tools PRIVATE include/pch.h)

# Post-build command to copy the lib
add_custom_command(TARGET Tools
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy
                   "$<TARGET_FILE:Tools>"
		   "${CMAKE_SOURCE_DIR}/common/$<TARGET_FILE_NAME:Tools>")