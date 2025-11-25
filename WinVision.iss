; WinVision installer template for Inno Setup CLI (iscc)

#define MyAppName "WinVision"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "WinVision Lab"
#define MyAppExeName "WinVisionApp.exe"

[Setup]
AppId={{05F1E7F2-7A2B-4AF7-8B4E-3D27BEF7F00A}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=Output
OutputBaseFilename=WinVisionSetup
Compression=lzma
SolidCompression=yes

[Files]
; Update the Source path to the location produced by your CI build
Source: "..\build\Release\WinVisionApp.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch {#MyAppName}"; Flags: postinstall skipifsilent