@echo off
gcc -Wall -Werror -g -o bin\\discon.exe src\\*.c -I "%LIBCURL%\\include\\curl" -L "%LIBCURL%\\lib" -lcurl