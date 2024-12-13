cmake_minimum_required(VERSION 3.10)
project(Launcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

file(GLOB_RECURSE HEADERS
    include/*.h
)

file(GLOB_RECURSE RESOURCES
    resources/*
)

# Define groups (filters)
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})
source_group("resources" FILES ${RESOURCES})

# Create the executable
add_executable(Launcher ${SOURCES} ${HEADERS} ${RESOURCES})

# Include directories
target_include_directories(Launcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Launcher PRIVATE ${CMAKE_SOURCE_DIR}/Tools/include)

# Link dependencies
add_dependencies(Launcher LauncherDll)
target_link_libraries(Launcher Tools)

# Set the icon for the executable
set_target_properties(Launcher PROPERTIES
    MACOSX_BUNDLE_ICON_FILE Launcher.ico
    WIN32_EXECUTABLE TRUE
)