cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MTIP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for the projects
add_subdirectory(LauncherDll)
add_subdirectory(Launcher)
add_subdirectory(Tools)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
	PROPERTY VS_STARTUP_PROJECT Launcher)