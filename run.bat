cls
@echo off

set old_dir=%cd%

cd "%~dp0"

Tools\make run

cd %old_dir%