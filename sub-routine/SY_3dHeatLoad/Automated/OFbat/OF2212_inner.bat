@echo off
if defined FOAM_ENV_SET goto :eof

rem =========== USER EDITABLE SETTINGS ===========
set MPI_ROOTDIR=C:\Program" "Files" "^(x86^)\OpenMPI_v1.6.2-x64
set PARAVIEW_HOME=F:\iso\ParaView-5.12.1
rem ==============================================
rem #FLAG#OF_dir
set of2406=C:\OpenFoam12qwe
set case = %CD%
if defined PARAVIEW_HOME set PATH=%PARAVIEW_HOME%\bin;%PATH%
if defined MPI_ROOTDIR set PATH=%MPI_ROOTDIR%\bin;%PATH%
REM set PATH=%~dp0..\lib\dummy;%~dp0..\lib\msmpi;%~dp0..\lib;%~dp0..\bin;%PATH%
set PATH=%of2406%\bin;%PATH%
set WM_PROJECT_DIR=%of2406%
set MPI_BUFFER_SIZE=20000000
set FOAM_ENV_SET=1
rem #FLAG#cd_calc_dir
cd C:\OpenFOAMCase\J20_inner_batchGUI33
testsh.bat
