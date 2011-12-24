
SplashTextOn("KCB Upgrade", "Checking running process.", -1, -1, -1, -1, 4, "", 24)
While ProcessExists("ioex-cs.exe")
    ProcessClose("ioex-cs.exe")
WEnd

$type = ""

uninstall("{A897C732-9F12-41F6-B0A1-901DE253FD37}","KCB10T")
uninstall("{4627B9F6-8EAA-46E1-ABCA-CAD3651DFB84}","KCB14T")

if $type = "" Then
	if 6 = MsgBox(4100, "Choose application","Press 'Yes' to install KCB10T application." & @CRLF &"Press 'No' to install KCB14T application", 0) Then
		$type = "KCB10T"
	Else
		$type = "KCB14T"
	EndIf
EndIf
	
if $type <> "" Then
	SplashTextOn("KCB Upgrade", "Installing " & $type & " new version", -1, -1, -1, -1, 4, "", 24)
;	ShellExecuteWait("msiexec.exe", "/i """ & @ScriptDir & "\\" & $type & "\\ioex-setup.msi""" & " /passive")
	RunWait("""" & @ScriptDir & "\\" & $type & "\\action.bat""",@ScriptDir,@SW_HIDE)
EndIf

SplashTextOn("KCB Upgrade", "Upgrade completed successfully!", -1, -1, -1, -1, 4, "", 24)
MsgBox(4096,"Reboot","Please reboot system after upgrade.")
SplashOff()
Shutdown(6)  ;Force a reboot


Func uninstall($pcode, $tvalue)
	While True
		$var = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\" & $pcode, "DisplayVersion")
		if $var <> "" Then
			$type = $tvalue
			SplashTextOn("KCB Upgrade", "Uninstalling " & $type & " old version " & $var, -1, -1, -1, -1, 4, "", 24)
;			ShellExecuteWait("msiexec.exe", "/x " & $pcode & " /passive")
			$var = ""
		EndIf
		if $var = "" Then
			ExitLoop
		EndIf
	WEnd
EndFunc
