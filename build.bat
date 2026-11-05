@echo off
echo =================================
echo   SRMS (Build Script)
echo =================================
echo.
echo Compiling source files...

gcc -std=c11 -Wall -Wextra -pedantic main.c student.c auth.c cli.c file_handling.c academic.c sorting.c statistics.c input_utils.c -o srms.exe

if %errorlevel% equ 0 (
    echo.
    echo BUILD SUCCESSFUL!
    echo Run using: srms.exe
) else (
    echo.
    echo BUILD FAILED! Check the errors above.
)

pause