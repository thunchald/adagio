@echo off

mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\adagio\code\win32_adagio.cpp user32.lib Gdi32.lib Opengl32.lib glu32.lib ..\adagio\code\glew\lib\release\x64\glew32.lib
popd
