@echo off
echo Est� a punto de sobreescribir la versi�n C++Builder con la versi�n Visual-C++
pause
copy *.c ..\grealce_CB\*.cpp
copy *.h ..\grealce_CB\*.h
copy defs_h_cbuilder.txt ..\grealce_cb\defs.h
copy defs.h defs_h_cvip.txt

