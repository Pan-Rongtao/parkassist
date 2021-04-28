@echo off

set msbuild_execute="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\msbuild.exe"
set build_dir="build"
if not exist "%build_dir%" md "%build_dir%"
cd "%build_dir%"

cmake -DCMAKE_INSTALL_PREFIX=../../../build/win32 -G "Visual Studio 16 2019" -A Win32 ..
@%msbuild_execute% INSTALL.vcxproj /p:Configuration="debug"
@%msbuild_execute% INSTALL.vcxproj /p:Configuration="release"

@pause