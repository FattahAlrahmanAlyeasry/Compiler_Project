# Makefile for Arabic Programming Language Compiler
# Compatible with Visual Studio 2015 and Windows

# Configuration
CONFIGURATION ?= Debug
PLATFORM ?= Win32
BUILD_DIR = Output/$(PLATFORM)/$(CONFIGURATION)
COMPILER_CORE_DLL = $(BUILD_DIR)/CompilerCore.dll
GUI_EXE = $(BUILD_DIR)/ArabicCompiler.GUI.exe

# Visual Studio paths (adjust these for your installation)
VS2015_PATH = "C:\Program Files (x86)\Microsoft Visual Studio 14.0"
MSBUILD = $(VS2015_PATH)\MSBuild\14.0\Bin\MSBuild.exe
DEVENV = $(VS2015_PATH)\Common7\IDE\devenv.exe

# Default target
all: build

# Build the entire project
build: build-compiler build-gui
	@echo Build completed successfully!

# Build the C++ compiler core
build-compiler: $(COMPILER_CORE_DLL)

$(COMPILER_CORE_DLL): CompilerCore/CompilerCore.vcxproj
	@echo Building CompilerCore...
	@if exist "$(MSBUILD)" ( \
		"$(MSBUILD)" CompilerCore/CompilerCore.vcxproj /p:Configuration=$(CONFIGURATION) /p:Platform=$(PLATFORM) /p:OutDir=..\$(BUILD_DIR)\ /p:IntDir=..\$(BUILD_DIR)\obj\ \
	) else ( \
		echo MSBuild not found. Please install Visual Studio 2015 or adjust VS2015_PATH \
		exit 1 \
	)

# Build the C# GUI
build-gui: $(GUI_EXE)

$(GUI_EXE): GUI/ArabicCompiler.GUI.csproj $(COMPILER_CORE_DLL)
	@echo Building GUI...
	@if exist "$(MSBUILD)" ( \
		"$(MSBUILD)" GUI/ArabicCompiler.GUI.csproj /p:Configuration=$(CONFIGURATION) /p:Platform=AnyCPU /p:OutDir=..\$(BUILD_DIR)\ \
	) else ( \
		echo MSBuild not found. Please install Visual Studio 2015 or adjust VS2015_PATH \
		exit 1 \
	)

# Clean build artifacts
clean:
	@echo Cleaning build artifacts...
	@if exist "Output" rmdir /s /q "Output"
	@if exist "GUI\bin" rmdir /s /q "GUI\bin"
	@if exist "GUI\obj" rmdir /s /q "GUI\obj"
	@if exist "CompilerCore\Debug" rmdir /s /q "CompilerCore\Debug"
	@if exist "CompilerCore\Release" rmdir /s /q "CompilerCore\Release"
	@if exist "CompilerCore\x64" rmdir /s /q "CompilerCore\x64"
	@echo Clean completed!

# Run the application
run: build
	@echo Running the application...
	@if exist "$(GUI_EXE)" ( \
		"$(GUI_EXE)" \
	) else ( \
		echo Application not found. Please build first. \
	)

# Open solution in Visual Studio
open:
	@echo Opening solution in Visual Studio...
	@if exist "$(DEVENV)" ( \
		"$(DEVENV)" ArabicCompiler.sln \
	) else ( \
		echo Visual Studio not found. Please install Visual Studio 2015 or adjust VS2015_PATH \
	)

# Build and run tests
test: build
	@echo Running tests...
	@if exist "Tests\Makefile" ( \
		cd Tests && make test \
	) else ( \
		echo Tests not found or not configured. \
	)

# Install dependencies (placeholder)
install-deps:
	@echo Installing dependencies...
	@echo This is a placeholder. Please install Visual Studio 2015 manually.
	@echo Required components:
	@echo - Visual C++ 2015
	@echo - .NET Framework 4.5.2
	@echo - Windows SDK 8.1

# Help
help:
	@echo Available targets:
	@echo   all          - Build the entire project
	@echo   build        - Build the entire project
	@echo   build-compiler - Build only the C++ compiler core
	@echo   build-gui    - Build only the C# GUI
	@echo   clean        - Clean all build artifacts
	@echo   run          - Build and run the application
	@echo   open         - Open solution in Visual Studio
	@echo   test         - Build and run tests
	@echo   install-deps - Show dependency installation info
	@echo   help         - Show this help message
	@echo.
	@echo Configuration options:
	@echo   CONFIGURATION=Debug|Release (default: Debug)
	@echo   PLATFORM=Win32|x64 (default: Win32)
	@echo.
	@echo Examples:
	@echo   make build
	@echo   make CONFIGURATION=Release PLATFORM=x64 build
	@echo   make clean
	@echo   make run

# Phony targets
.PHONY: all build build-compiler build-gui clean run open test install-deps help

