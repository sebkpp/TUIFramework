@Echo off

::Reciever Port of the Server
set recieverPort=7998

::Sender Port of the Server
set senderPort=7999

::XML-Configuration-file
ECHO Press 0 to set the winMOD config, 1 to set the mouse config
SET /P xmlchoice=

IF %xmlchoice%==0 (
	set xmlConfiguration=./xmlconfig/TUIconfig_winMOD_wayBack.xml
) ELSE (
	set xmlConfiguration=./xmlconfig/TUIconfig_mouse.xml
	:: The Execution of the Mouse Server
	start ../../bin/Win32/14.0/x86/Debug/extensions/MouseServer/MouseServer.exe
)

::Path to DLL-Libaries
set libPath=../../bin/Win32/14.0/x86/Debug/lib/TUIPlugins/shared

:: The Execution of the TUI-Server
start "TUI Server" ../../bin/Win32/14.0/x86/Debug/tuiserver/TUIBinTUIServer.exe %recieverPort% %senderPort% %xmlConfiguration% %libPath%

:: The Execution of the Python Interface
start "Python Interface Output" python_d python_interface_JSON_UDP.py %xmlConfiguration%