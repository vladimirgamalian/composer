set output="..\..\src\gitver.h"

for /f "tokens=*" %%i in ('git describe master') do set version=%%i
set version_definition=#define GIT_VER ^"%version%^"

echo #pragma once > %output%
echo %version_definition% >> %output%
