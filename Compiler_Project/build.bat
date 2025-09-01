@echo off
echo Building Arabic Compiler Project...
echo.

REM Check if we're in the right directory
if not exist "CompilerCore" (
    echo Error: CompilerCore directory not found!
    echo Please run this script from the project root directory.
    pause
    exit /b 1
)

if not exist "GUI" (
    echo Error: GUI directory not found!
    echo Please run this script from the project root directory.
    pause
    exit /b 1
)

echo Creating Output directory...
if not exist "Output" mkdir Output
if not exist "Output\Win32" mkdir Output\Win32
if not exist "Output\Win32\Debug" mkdir Output\Win32\Debug

echo.
echo Project structure created successfully!
echo.
echo To build the project:
echo 1. Open ArabicCompiler.sln in Visual Studio 2015 or later
echo 2. Set Configuration to Debug and Platform to Win32
echo 3. Build the solution (Ctrl+Shift+B)
echo.
echo Or use the Makefile if you have make installed:
echo make build
echo.
echo The project is now ready for Visual Studio!
pause

