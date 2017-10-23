REM Make a proper cmake script later

for /f "tokens=1-4 delims=/ " %%i in ("%date%") do (
     set dow=%%i
     set month=%%j
     set day=%%k
     set year=%%l
   )
   set datestr=%month%_%day%_%year%
   echo datestr is %datestr%


rd /S /Q x64
rd /S /Q lbsa
rd /S /Q Debug
rd /S /Q Release

MSBuild utl-main.sln /t:Clean,Rebuild /p:Configuration=Release,Platform=x64
MSBuild utl-main.sln /t:Clean,Rebuild /p:Configuration=Release,Platform=Win32

mkdir lbsa

;;x86
winrar a -afzip lbsa\data.lbsa res
copy Release\lbsa-main.exe lbsa\
copy Release\*.dll lbsa\
copy consolab.ttf lbsa\
winrar a -afzip -df lbsa\lbsa-%datestr%-win32.zip lbsa\*.dll lbsa\*.exe lbsa\*.ttf lbsa\*.utl

;;x64
winrar a -afzip lbsa\data.lbsa res
copy x64\Release\lbsa-main.exe lbsa\
copy x64\Release\*.dll lbsa\
copy consolab.ttf lbsa\
winrar a -afzip -df lbsa\lbsa-%datestr%-win64.zip lbsa\*.dll lbsa\*.exe lbsa\*.ttf lbsa\*.utl

git log -1 --pretty=%%B > lbsa\COMMIT.log

