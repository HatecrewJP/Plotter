@echo off
setlocal enabledelayedexpansion

@REM Find Visual Studio Installation Path
for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath') do set VS_PATH=%%i
if "!VS_PATH!" equ "" (
echo No VS installation found
exit \b 1
)

@REM Initialize Environment Variable Cache
set ARCH=x64
if not exist misc mkdir misc

set CL_CACHE="%~dp0misc\cl_cache.bat"
if exist %CL_CACHE% (
    call %CL_CACHE%
) else (
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall" %ARCH%
echo set INCLUDE=!INCLUDE!> 	%CL_CACHE%
echo set LIB=!LIB!>>		%CL_CACHE%
echo set PATH=!PATH!>>		%CL_CACHE%
)


@REM Compile

set Root=%~dp0
set BuildDir=bin
set CodePath=%Root%\src
set AdditionalLibsPath=%Root%\lib\

set Libraries=kernel32.lib user32.lib shell32.lib Winmm.lib gdi32.lib opengl32.lib %AdditionalLibsPath%\raylib.lib
set CommonCompilerFlags=-MD -nologo -GR-  -Oi -Od -W4 -WX -wd4201 -wd4100 -wd4101 -wd4189  -DHANDMADE_SLOW=1 -DHANDMADE_INTERNAL=1 -DHANDMADE_WIN32=1  -FC -Zi -std:c11
set CommonLinkerFlags=%Libraries% -opt:ref -incremental:no 

if not exist %BuildDir% mkdir %BuildDir%
pushd %BuildDir%
cl %CommonCompilerFlags% %CodePath%/main.c /link %CommonLinkerFlags% 
popd