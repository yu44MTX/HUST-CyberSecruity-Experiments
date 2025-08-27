.386
.model flat,stdcall
option casemap:none

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\kernel32.lib
include \masm32\include\user32.inc
includelib \masm32\lib\user32.lib

.data
MsgBoxCaption   db  "hello,hy!",0
MsgBoxText1     db  "U202112146 ºúÓî PEHost:MessageBox1",0
MsgBoxText2     db  "U202112146 ºúÓî PEHost:MessageBox2",0

.code
start:
invoke MessageBox,NULL,addr MsgBoxText1,addr MsgBoxCaption,MB_OK+MB_ICONASTERISK+MB_DEFBUTTON1+MB_SYSTEMMODAL
invoke MessageBox,NULL,addr MsgBoxText2,addr MsgBoxCaption,MB_OK+MB_ICONASTERISK+MB_DEFBUTTON1+MB_SYSTEMMODAL
invoke ExitProcess,NULL
end start