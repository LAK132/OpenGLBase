@echo off
SetLocal EnableDelayedExpansion

set mode=%1
set target=%2

if not "%mode%"=="debug" if not "%mode%"=="release" if not "%mode%"=="clean" goto useage
if not "%target%"=="x86" if not "%target%"=="x64" if not "%mode%"=="clean" goto useage

call makelist.bat %target%

if "%mode%"=="clean" goto clean

echo Compiling in %mode% mode for %target%

title Compiler

REM some windows functions are pedantic about \
set OUTDIR=!OUTDIR!\%mode%\%target%
set LIBDIR=!LIBDIR!\%target%
set OUT=%OUTDIR%\%APP%

if not exist %OUTDIR% mkdir %OUTDIR%

set _LIBS=
for %%L in (%LIBS%) do (
    set _LIBS=!_LIBS! %LIBDIR%/%%L
)

if "%mode%"=="debug" goto debug
if "%mode%"=="release" goto release

:useage
echo compile: "make [debug/release] [x86/x64]"
echo clean: "make clean"
goto :eof

:clean
for /f %%F in ('dir /b %BINDIR%') do (
    if "%%~xF"==".obj" del %BINDIR%\%%F
)
goto :eof

:release
set COMPCOM=-DNDEBUG /bigobj
set LINKCOM=/SUBSYSTEM:CONSOLE
goto run

:debug
set COMPCOM=-Zi /bigobj
set LINKCOM=/SUBSYSTEM:CONSOLE /DEBUG
goto run

:run
set allobj=
for %%P in (%SOURCES%) do (
    for %%O in (!%%P_OBJ!) do (
        set allobj=!allobj! %BINDIR%/%%O.obj
        set inc=
        for %%I in (!%%P_INC!) do (set inc=!inc! /I%%I)
        call cl /std:%CPPVER% /nologo /MD -EHsc %COMPOPTS% %COMPCOM% /Fo:%BINDIR%/%%O.obj /c !%%P_SRC!/%%O !inc!
    )
)
call link /nologo %LINKCOM% /out:%OUT% %allobj% %_LIBS%
for /f %%F in ('dir /b %LIBDIR%') do (
    if "%%~xF"==".dll" echo f | xcopy /y %LIBDIR%\%%F %OUTDIR%\%%F
)
goto :eof