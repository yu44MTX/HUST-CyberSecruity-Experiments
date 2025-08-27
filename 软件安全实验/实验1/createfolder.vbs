WScript.Echo("HelloToVBS")
dim newdir
set newdir=wscript.createobject("scripting.filesystemobject")
for k=1 to 3
	anewfolder="chapter"&k
	newdir.createfolder(anewfolder)
next