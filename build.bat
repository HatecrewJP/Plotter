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

set Libraries= %AdditionalLibsPath%\raylibdll.lib
set CommonCompilerFlags=-MTd -nologo -GR-  -Oi -Od -W4 -WX -wd4201 -wd4100 -wd4101 -wd4189 -FC -Zi 
set CommonLinkerFlags=%Libraries% -opt:ref -incremental:no

if not exist %BuildDir% mkdir %BuildDir%
pushd %BuildDir%
cl %CommonCompilerFlags% %CodePath%/main.c /link -opt:ref -incremental:no %Libraries% kernel32.lib

echo LOCK > lock.tmp

cl %CommonCompilerFlags% %CodePath%/app.c /LD /link -incremental:no /PDB:app.pdb /EXPORT:UpdateApp %Libraries%

del lock.tmp


popd
