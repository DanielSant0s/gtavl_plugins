@echo off
rem Copyright (C):  2001, 2002  Earnie Boyd
rem   mailto:earnie@users.sf.net
rem This file is part of Minimal SYStem
rem   http://www.mingw.org/msys.shtml
rem
rem File:	    msys.bat
rem Revision:	    2.0
rem Revision Date:  April 17th, 2002

rem ember to set the "Start in:" field of the shortcut.
rem A value similar to C:\msys\1.0\bin is what the "Start in:" field needs
rem to represent.

rem ember value of GOTO: is used to know recursion has happened.
if "%1" == "GOTO:" goto %2

rem ember command.com only uses the first eight characters of the label.
goto _WindowsNT

rem ember that we only execute here if we are in command.com.
:_Windows

if "x%COMSPEC%" == "x" set COMSPEC=command.com
start %COMSPEC% /e:4096 /c %0 GOTO: _Resume %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
goto EOF

rem ember that we execute here if we recursed.
:_Resume
for %%F in (1 2 3) do shift

rem ember that we get here even in command.com.
:_WindowsNT

if "x%MSYSTEM%" == "x" set MSYSTEM=MINGW32
if "%1" == "MSYS" set MSYSTEM=MSYS

if NOT "x%DISPLAY%" == "x" set DISPLAY=

if EXIST bin\nul cd bin
if EXIST rxvt.exe goto startrxvt
if EXIST sh.exe goto startsh

echo Cannot find the rxvt.exe or sh.exe binary -- aborting.
pause
exit 1

rem If you don't want to use rxvt then rename the file rxvt.exe to something
rem else.  Then sh.exe will be used instead.
:startrxvt

rem Setup the default colors for rxvt.
if "x%MSYSBGCOLOR%" == "x" set MSYSBGCOLOR=White
if "x%MSYSFGCOLOR%" == "x" set MSYSFGCOLOR=Black
if "x%MINGW32BGCOLOR%" == "x" set MINGW32BGCOLOR=LightYellow
if "x%MINGW32FGCOLOR%" == "x" set MINGW32FGCOLOR=Navy
if "%MSYSTEM%" == "MSYS" set BGCOLOR=%MSYSBGCOLOR%
if "%MSYSTEM%" == "MSYS" set FGCOLOR=%MSYSFGCOLOR%
if "%MSYSTEM%" == "MINGW32" set BGCOLOR=%MINGW32BGCOLOR%
if "%MSYSTEM%" == "MINGW32" set FGCOLOR=%MINGW32FGCOLOR%

start rxvt -backspacekey  -sl 2500 -fg %FGCOLOR% -bg %BGCOLOR% -sr -fn Courier-12 -tn msys -geometry 80x25 -e /bin/sh --login -i
exit

:startsh
start sh --login -i
exit

:EOF

