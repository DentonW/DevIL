@echo off
echo Generating include/IL/config.h based on what your system has installed...
copy include\IL\config.h.win include\IL\config.h
echo Done.
echo.
echo Please check include/IL/config.h to make sure that it has
echo  set the appropriate #defines.
echo.
pause
