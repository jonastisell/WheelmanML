@echo off
call "E:\_Software\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cl /EHsc /LD ./src/dllmain.cpp ./src/wheelmanml.cpp ./src/asi_loader.cpp ./src/logger.cpp /link /DEF:./src/exports.def /OUT:./out/xinput9_1_0.dll
PAUSE