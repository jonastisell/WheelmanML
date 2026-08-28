@echo off
call "E:\_Software\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cl /LD ./src/dllmain.cpp /link /DEF:./src/exports.def /OUT:./out/xinput9_1_0.dll