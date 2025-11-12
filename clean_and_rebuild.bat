@echo off
echo Nettoyage du projet DriveSmart...
echo.

REM Supprimer les fichiers de build
if exist build rmdir /s /q build
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release
if exist *.o del /q *.o
if exist moc_*.cpp del /q moc_*.cpp
if exist ui_*.h del /q ui_*.h
if exist qrc_*.cpp del /q qrc_*.cpp
if exist Makefile del /q Makefile
if exist Makefile.* del /q Makefile.*

echo Nettoyage termine.
echo.
echo Vous pouvez maintenant recompiler le projet dans Qt Creator.
echo Utilisez: Build -> Clean All, puis Build -> Rebuild All
pause

