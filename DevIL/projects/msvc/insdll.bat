if "%ILNOCOPY%" == "" goto work
goto done

:work

if "%SystemRoot%" == "" goto isnotnt

echo Copying %1 to %SystemRoot%\System32...
copy %1 %SystemRoot%\System32
goto done

:isnotnt
if "%WINDIR%" == "" goto isnot95

echo Copying %1 to %WINDIR%\System...
copy %1 %WINDIR%\System
goto done

:isnot95
echo *** Unrecognised platform! ***
goto done

:done
