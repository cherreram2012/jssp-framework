@echo off
ECHO �����������������������������������������������������������������������Ŀ
ECHO ����������                      CleanerX                       ����������
ECHO �����������������������������������������������������������������������Ĵ
ECHO �����������������������������������������������������������������������Ĵ
ECHO �                                                                       �
ECHO ���  This batch file deletes the useless files in the projects folder ���
ECHO �                                                                       �
ECHO �    Languages supported:                                               �
ECHO �    � Borland C++ Builder                                              �
ECHO �    � Microsoft Visual Studio.NET                                      �
ECHO �                                                                       �
ECHO �   Thanks For � Read me � mailto:eperezm@informatica.udg.co.cu         �
ECHO �                                                                       �
ECHO �����������������������������������������������������������������������Ĵ
ECHO �������������������������������������������������������������������������
echo.

if not exist WinXP.obj goto PROCEED
ren WinXP.obj WinXP._obj

:PROCEED
if not exist *.obj goto NEXT
echo C++ object files found
del *.obj

:NEXT
if not exist *.~* goto NEXT
echo C++ backup files found
del *.~*

:NEXT
if not exist *.tds goto NEXT
echo C++ setting file found
del *.tds

:NEXT
if not exist WinXP._obj goto BIN
ren WinXP._obj WinXP.obj

:BIN
if not exist bin goto OBJ
cd bin
if not exist Debug goto NEXT
echo C# bin/Debug folder found
RMDIR /S /Q Debug

:NEXT
if not exist Release goto NEXT
echo C# bin/Release folder found
RMDIR /S /Q Release

:NEXT
cd..

:OBJ
if not exist obj goto END

cd obj
if not exist Debug goto NEXT
echo C# obj/Debug folder found
RMDIR /S /Q Debug

:NEXT
if not exist Release goto NEXT
echo C# obj/Release folder found
RMDIR /S /Q Release

:NEXT
cd..

:END
echo.
echo DONE !!            (any change to this file will be considered terrorism)
pause >nul