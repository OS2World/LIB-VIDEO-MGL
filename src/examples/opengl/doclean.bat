@echo off
rem Batch file to clean all IDE directories

cd %1

cd bc4-ide
except (*.ide) del /y .

cd ..\bc5-ide
except (*.ide) del /y .

cd ..\sc-ide
except (*.opn *.prj) del /y .

cd ..\vc-ide\win32
except (*.mak *.mdp *.ncb) del /y .
deltree /y debug
cd ..\win32dll
except (*.mak *.mdp *.ncb) del /y .
deltree /y debug

cd ..\..\wc-ide
except (*.wpj *.tgt) del /y .
cd ..\..

