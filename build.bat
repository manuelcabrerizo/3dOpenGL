@echo off

SET LIBS=gdi32.lib kernel32.lib user32.lib winmm.lib opengl32.lib glu32.lib glfw3dll.lib
SET OPGL_LIB_PATH="C:\glfw3\lib"
SET OPGL_INCLUDE_PATH="C:\glfw3\include"

IF NOT EXIST build mkdir build

pushd build
cl /EHsc /Zi ..\code\*.cpp ..\code\glad.c /Fe:game %LIBS% ^
/I%OPGL_INCLUDE_PATH% /I"D:\Dev\OpenGL\include" ^
/link /LIBPATH:%OPGL_LIB_PATH% /SUBSYSTEM:CONSOLE 
popd
