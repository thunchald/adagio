@echo off

REM
REM  To run this at startup, use this as your shortcut target:
REM  %windir%\system32\cmd.exe /k v:\handmade\misc\shell.bat
REM
subst v: d:\projects
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
set path=v:\adagio\misc;%path%
