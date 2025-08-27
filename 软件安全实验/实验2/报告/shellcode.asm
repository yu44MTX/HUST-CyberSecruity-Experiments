.386
.model flat, stdcall

.code

_start:
xor eax,eax
push eax
mov eax,6578652Eh ;".exe"
push eax
mov eax,636C6163h ;"calc"
push eax
mov eax,esp
push eax
mov eax,771D4720h  ;"system"
call eax
xor eax,eax
push eax 
mov eax,771F7830h  ;"exit"

end _start