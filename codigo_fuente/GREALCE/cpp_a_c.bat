@echo off
echo Está a punto de sobreescribir la versión Visual-C++ con la versión C++Builder
pause
copy ..\grealce_CB\*.cpp *.c
copy ..\grealce_CB\*.h *.h
copy defs_h_cvip.txt defs.h
copy ..\grealce_cb\defs.h defs_h_cbuilder.txt 
