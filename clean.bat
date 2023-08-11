cls
@echo off

set old_dir=%cd%

cd "%~dp0"

Tools\make clean

cd %old_dir%