Dim WshShell  
set WshShell = CreateObject("wscript.Shell")  
WshShell.Run "cmd /k"  
for i = 1 to 3  
WScript.Sleep 500  
WshShell.SendKeys "net user admin" & i & " abcd@123 /add"  
WshShell.SendKeys "{ENTER}"  
next  
WshShell.SendKeys "exit"  
WshShell.SendKeys "{ENTER}" 