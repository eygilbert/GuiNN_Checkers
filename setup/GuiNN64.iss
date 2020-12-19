; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
#define AppVersion "2.05"
#define AppEnvironment "64"

AppName=GuiNN_Checkers{#AppEnvironment}
AppVerName=GuiNN_Checkers{#AppEnvironment} version {#AppVersion}
DefaultDirName={commonpf32}\CheckerBoard
DisableDirPage=no
AppendDefaultDirName=no
DefaultGroupName=GuiNN_Checkers
DisableProgramGroupPage=yes
InfoBeforeFile=setupmessage.txt
OutputDir=Output
OutputBaseFilename=gui_nn{#AppEnvironment}_{#AppVersion}_setup
Compression=lzma/ultra
SolidCompression=yes
Uninstallable=no
DirExistsWarning=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\checkerboard\egdb64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\checkerboard\engines\gui_nn{#AppEnvironment}.dll"; DestDir: "{app}\engines"; Flags: ignoreversion
Source: "..\bin\opening.gbk"; DestDir: "{app}\engines"; Flags: ignoreversion
Source: "..\bin\nets.gnn"; DestDir: "{app}\engines"; Flags: ignoreversion
