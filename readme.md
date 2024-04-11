Laboratory work for University

1. Compile only through visual studio project (idk why, but otherwise there so many extra sections and probably, when you build your project through vscode, there is defence from disassembling)

2. Disassembly - use vs utility called "dumpbin" like that ( dumpbin /SECTION:.text /DISASM check.exe > code-text.txt )

   2.1 ".text" is a section where text of source code is stored

   2.2 "code-text.txt" file that will contain your disassembled code

4. Reveal address that you needed through dumpbin then edit assembly code inside .exe through Hiew (something like test eax eax -> xor eax eax)

5. When you will be testing hacked programm, don't forget to add "password1 > psw.txt" in working directory (where .
exe is stored) 

6. Obfuscator works with "obf_config.cfg" file and accepts code that will be obfuscated through cmd arguments like "./obfuscator check.c"