@echo off
setlocal enabledelayedexpansion

REM Define the source and build directories
set SRC_DIR=src
set BUILD_DIR=build

REM Check if the build directory exists, if not, create it
if not exist "!BUILD_DIR!" mkdir "!BUILD_DIR!"

REM Initialize variables to hold CPP and OBJ files paths
set "CPP_FILES="
set "OBJ_FILES="

REM Find all CPP files and compile them individually to OBJ files
for /r %%i in (*.cpp) do (
    REM Extract the filename without extension
    set "FILE_NAME=%%~ni"
    
    REM Compile the CPP file to an OBJ file in the build directory
    cl /c /EHsc /std:c++17 "%%i" /Fo"!BUILD_DIR!\!FILE_NAME!.obj"
    
    REM Check if the compilation was successful (errorlevel 0 means success)
    if NOT !errorlevel! == 0 (
        echo Compilation failed, please check the error messages.
        exit /b !errorlevel!
    )
    
    REM Append the OBJ file path to the list
    set "OBJ_FILES=!OBJ_FILES! !BUILD_DIR!\!FILE_NAME!.obj"
)

REM Link all OBJ files into a single executable named main.exe
cl /EHsc /Fe:main.exe !OBJ_FILES!

REM Check if the link was successful (errorlevel 0 means success)
if %errorlevel% == 0 (
    echo Link successful, running main.exe...
    main.exe
) else (
    echo Link failed, please check the error messages.
)

REM Pause the script to see the output
pause
