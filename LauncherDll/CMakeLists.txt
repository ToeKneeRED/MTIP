cmake_minimum_required(VERSION 3.10)
project(LauncherDll)

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

# Create the DLL
add_library(LauncherDll SHARED ${SOURCES} ${HEADERS})

# Specify the include directory for this library
target_include_directories(LauncherDll PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories
target_include_directories(LauncherDll PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(LauncherDll PRIVATE ${CMAKE_SOURCE_DIR}/Tools/include)

# Add precompiled headers if needed
# target_precompile_headers(LauncherDll PRIVATE include/pch.h)

target_link_libraries(LauncherDll Tools)

# Post-build command to copy the DLL
add_custom_command(TARGET LauncherDll
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy
                   "$<TARGET_FILE:LauncherDll>"
		   "${CMAKE_SOURCE_DIR}/common/$<TARGET_FILE_NAME:LauncherDll>")
                   # COMMAND ${CMAKE_COMMAND} -E copy /Y "$(TargetPath)" "$(SolutionDir)dll\\$(TargetFileName)")