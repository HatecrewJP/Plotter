@echo off
pushd bin
del /Q *.exp
del /Q *.pdb
del /Q *.obj
del /Q *.exe
del /Q app.dll
del /Q app.lib

popd