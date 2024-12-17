@echo off

if exist "build" (
    echo Removing build folder...
    rmdir /s /q "build"
)

echo Recreating build folder...
mkdir "build"

if exist "vsxmake2022" (
    echo Removing vsxmake2022 folder...
    rmdir /s /q "vsxmake2022"
)

echo Generating project...
xmake project -k vsxmake

echo Building project...
xmake -y

pause