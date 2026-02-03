@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

cl /EHsc Main.cpp proc.cpp /Fe:..\Debug\ac_ext.exe /Fo:..\bin\intermediates\Win32\Debug\
