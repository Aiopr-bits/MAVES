@echo off

if exist "%~dp0custom.bat" (
  call "%~dp0custom.bat" %1 %2 %3
)

call "C:/Users/Aiopr/Desktop/OCCT-7_8_0/install\.\env.bat" %1 %2 %3
if /I ["%1"] == ["vc141"] set "VCVER=vc141"
if /I ["%1"] == ["vc142"] set "VCVER=vc142"
if /I ["%1"] == ["vc143"] set "VCVER=vc143"
set "BIN_DIR=win%ARCH%\%VCVER%\bind"
set "LIB_DIR=win%ARCH%\%VCVER%\libd"

if ["%CASDEB%"] == [""] (
  set "BIN_DIR=win%ARCH%\%VCVER%\bin"
  set "LIB_DIR=win%ARCH%\%VCVER%\lib"
)

set "PATH=%~dp0%BIN_DIR%;%PATH%"

if not "%QTDIR%" == "" (
  set "RES_DIR=%~dp0win%ARCH%\%VCVER%\res"

  set "CSF_ResourcesDefaults=!RES_DIR!"
  set "CSF_TutorialResourcesDefaults=!RES_DIR!"
  set "CSF_IEResourcesDefaults=!RES_DIR!"

  set "PATH=%QTDIR%/bin;%PATH%"
  set "QT_QPA_PLATFORM_PLUGIN_PATH=%QTDIR%\plugins\platforms"
)

set "CSF_OCCTOverviewSampleCodePath=%~dp0..\..\OCCTOverview\code"
