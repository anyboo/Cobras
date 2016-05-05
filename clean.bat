@echo off
setlocal
call "%VS120COMNTOOLS%vsvars32.bat
MSBUILD Cobras.sln /t:Clean /p:Configuration=Debug
MSBUILD Cobras.sln /t:Clean /p:Configuration=Release

RD /S /Q  gCatch\Debug
RD /S /Q  gCatch\Release

RD /S /Q  Verifier\Debug
RD /S /Q  Verifier\Release

exit /b %ERRORLEVEL%
endlocal
pause