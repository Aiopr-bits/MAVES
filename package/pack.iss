 ;/*--------------------------------*- C++ -*----------------------------------*\
 ;| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
 ;| Author: Zhiyong Zeng                                                        |
 ;| Date: December 01, 2024                                                     |
 ;| Email: mailzengzhiyong@gamil.com                                            |
 ;\*---------------------------------------------------------------------------*/

#define MyAppName "MAVES"
#define MyAppVersion "20250228"
#define MyAppExeName "MAVES.exe"

[Setup]
; 注意：AppId 的值唯一标识此应用程序.不要在其他应用程序的安装程序中使用相同的 AppId 值.
; (若要生成新的 GUID，请单击"工具" - "生成 GUID").
AppId={{327EB05A-073C-490B-8DB8-9BF2F2F67A0E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={autopf}\{#MyAppName}
; "ArchitecturesAllowed=x64compatible" specifies that Setup cannot run
; on anything but x64 and Windows 11 on Arm.
ArchitecturesAllowed=x64compatible
; "ArchitecturesInstallIn64BitMode=x64compatible" requests that the
; install be done in "64-bit mode" on x64 or Windows 11 on Arm,
; meaning it should use the native 64-bit Program Files directory and
; the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
;InfoAfterFile=..\README.txt
; 取消下列注释行，在非管理员安装模式下运行(仅为当前用户安装.)
;PrivilegesRequired=lowest
OutputDir=..\package
OutputBaseFilename=MAVES
SetupIconFile=..\res\icon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "chs"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\x64\bin\{#MyAppExeName}"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "..\x64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; 注意:  在任何共享系统文件上不要使用 "Flags: ignoreversion"

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

