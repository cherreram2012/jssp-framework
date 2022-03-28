@echo off
ECHO ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  
ECHO ³²²±±°°°°°          Limpia Proyectos de C++ Builder 6.0        °°°°°±±²²³
ECHO ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
ECHO ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
ECHO ³                                                                       ³
ECHO ³    ¯¯  Este .bat elimina de un proyecto de C++ Builder 6.0       ®®   ³
ECHO ³       todos los ficheros temporales.                                  ³
ECHO ³                                                                       ³
ECHO ³     Carpetas analizadas:           Ficheros eliminados:               ³
ECHO ³       ¯ clases                       ¯ .tds                           ³
ECHO ³       ¯ formularios                  ¯ .obj                           ³
ECHO ³                                      ¯ .~h                            ³
ECHO ³                                      ¯ .~cpp                          ³
ECHO ³                                      ¯ .~dfm                          ³
ECHO ³                                      ¯ .~ddp                          ³
ECHO ³                                      ¯ .~bpr                          ³
ECHO ³                                                                       ³
ECHO ³                                                                       ³
ECHO ³   Autor : Carlos Herrera Marquez                                      ³
ECHO ³   e-mail: cherreram@informatica.udg.co.cu                             ³ 
ECHO ³   Curso 2006-2007                                                     ³
ECHO ³                                                                       ³
ECHO ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
ECHO ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo.

rem ------------------------ ELIMINA LOS .TDS ---------------------------------------
rem ---BUSCA .TDS EN LA  RAIZ DEL PROYECTO---
if not exist *.tds goto NO_TDS
echo C++ setting found in root 
del *.tds
echo.
goto OBJ

:NO_TDS
echo C++ setting no found in root
echo.
goto OBJ

rem ------------------------ ELIMINA LOS .OBJ ---------------------------------------
rem ---BUSCA .OBJ EN LA  RAIZ DEL PROYECTO---
:OBJ
if not exist *.obj goto NO_OBJ_IN_ROOT
echo C++ object files found in root 
del *.obj
goto NEXT

:NO_OBJ_IN_ROOT
echo C++ object files no found in root
goto NEXT

rem ---BUSCA .OBJ EN LA CARPETA CLASES---
:NEXT
cd clases
if not exist *.obj goto NO_OBJ_IN_CLASES
echo C++ object files found in folder clases
del *.obj
goto NEXT

:NO_OBJ_IN_CLASES
echo C++ object files no found in folder clases
goto NEXT

rem ---BUSCA .OBJ EN LA CARPETA FORMULARIOS---
:NEXT
cd..
cd formularios
if not exist *.obj goto NO_OBJ_IN_FORMULARIOS
echo C++ object files found in folder formularios
del *.obj
echo.
goto NEXT

:NO_OBJ_IN_FORMULARIOS
echo C++ object files no found in folder formularios
echo.
goto NEXT

rem ------------------------ ELIMINA LOS .~h ---------------------------------------
rem ---BUSCA .~H EN LA RAIZ DEL PROYECTO---
:NEXT
cd.. 
if not exist *.~h goto NO_~H_IN_ROOT
echo .~h files found in root
del *.~h
goto NEXT

:NO_~H_IN_ROOT
echo .~h files no found in root
goto NEXT

rem ---BUSCA .~H EN LA CARPETA CLASES---
:NEXT
cd clases
if not exist *.~h goto NO_~H_IN_CLASES
echo .~h files found in folder clases
del *.~h
goto NEXT

:NO_~H_IN_CLASES
echo .~h files no found in folder clases
goto NEXT

rem ---BUSCA .~H EN LA CARPETA FORMULARIOS---
:NEXT
cd..
cd formularios
if not exist *.~h goto NO_~H_IN_FORMULARIOS
echo .~h files found in folder formularios
del *.~h
echo.
goto NEXT

:NO_~H_IN_FORMULARIOS
echo .~h files no found in folder formularios
echo.
goto NEXT

rem ------------------------ ELIMINA LOS .~cpp ---------------------------------------
rem ---BUSCA .~CPP EN LA RAIZ DEL PROYECTO---
:NEXT
cd..
if not exist *.~cpp goto NO_~CPP_IN_ROOT
echo .~cpp files found in folder clases
del *.~cpp
goto NEXT

:NO_~CPP_IN_ROOT
echo .~cpp files no found in folder clases
goto NEXT

rem ---BUSCA .~CPP EN LA CARPETA CLASES---
:NEXT
cd clases
if not exist *.~cpp goto NO_~CPP_IN_CLASES
echo .~cpp files found in folder clases
del *.~cpp
goto NEXT

:NO_~CPP_IN_CLASES
echo .~cpp files no found in folder clases
goto NEXT

rem ---BUSCA .~CPP EN LA CARPETA FORMULARIOS---
:NEXT
cd..
cd formularios
if not exist *.~cpp goto NO_~CPP_IN_FORMULARIOS
echo .~cpp files found in folder formularios
del *.~cpp
echo.
goto NEXT

:NO_~CPP_IN_FORMULARIOS
echo .~cpp files no found in folder formularios
echo.
goto NEXT

rem ------------------------ ELIMINA LOS .~dfm ---------------------------------------
rem ---BUSCA .~DFM EN LA CARPETA FORMULARIOS---
:NEXT
cd..
cd formularios
if not exist *.~dfm goto NO_~DFM_IN_FORMULARIOS
echo .~dfm files found in folder formularios
del *.~dfm
echo.
goto NEXT

:NO_~DFM_IN_FORMULARIOS
echo .~dfm files no found in folder formularios
echo.
goto NEXT

rem ------------------------ ELIMINA LOS .~ddp ---------------------------------------
rem ---BUSCA .~DDP EN LA CARPETA FORMULARIOS---
:NEXT
cd..
cd formularios
if not exist *.~ddp goto NO_~DDP_IN_FORMULARIOS
echo .~ddp files found in folder formularios
del *.~ddp
echo.
goto NEXT

:NO_~DDP_IN_FORMULARIOS
echo .~ddp files no found in folder formularios
echo.
goto NEXT

rem ------------------------ ELIMINA LOS .BPR ---------------------------------------
rem ---BUSCA .~BPR EN LA RAIZ DEL PROYECTO---
:NEXT
cd..
if not exist *.~bpr goto NO_~BPR_IN_ROOT
echo .~bpr files found in root
del *.~bpr
goto NEXT

:NO_~BPR_IN_ROOT
echo .~bpr files no found in root
goto NEXT

:NEXT
echo.
pause 