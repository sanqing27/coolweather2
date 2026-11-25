@echo off
setlocal
cd /d "%~dp0"
echo [1/2] Combining chunks...
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0combine.ps1"
if not exist "%~dp0study_restored.7z" ( echo Combine failed & pause & exit /b 1 )
echo [2/2] Extracting...
set "SEVEN=C:\Program Files\7-Zip\7z.exe"
if not exist "%SEVEN%" set "SEVEN=7z.exe"
"%SEVEN%" x "%~dp0study_restored.7z" -o"%~dp0study" -y
if errorlevel 1 ( echo Extract failed, please open study_restored.7z manually & pause & exit /b 1 )
echo Done. Extracted to: %~dp0study
pause
