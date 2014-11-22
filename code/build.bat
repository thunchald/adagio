@echo off

mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\adagio\code\win32_adagio.cpp
popd
