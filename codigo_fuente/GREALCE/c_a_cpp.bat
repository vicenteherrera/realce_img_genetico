@echo off
echo Está a punto de sobreescribir la versión C++Builder con la versión Visual-C++
pause
copy *.c ..\grealce_CB\*.cpp
copy *.h ..\grealce_CB\*.h
copy defs_h_cbuilder.txt ..\grealce_cb\defs.h
copy defs.h defs_h_cvip.txt

