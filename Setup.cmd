@echo off
rem Quick start: checks prerequisites, finds your ROMs, builds, and makes a launcher.
rem Everything it does is described in README.md; details go to setup.log.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\setup.ps1" %*
if errorlevel 1 pause
