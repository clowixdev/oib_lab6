# Laboratory work for University

### Dissassembling and obfuscating

General steps to get all that you need

1. Compile only through visual studio project
      
        idk why, but otherwise there so many extra sections and probably, when you build
        your project through vscode, there is defence from disassembling
2. Disassembly - use vs utility called "dumpbin" like that ( dumpbin /SECTION:.text /DISASM check.exe > code-text.txt )

   		- ".text" is a section where text of source code is stored
		- "code-text.txt" file that will contain your disassembled code
                  
3. Reveal address that you needed through dumpbin

		then edit assembly code inside .exe through Hiew (something like test eax eax -> xor eax eax)

4. When you will be testing hacked programm, don't forget to add in working directory (where .exe is stored) 

   		password1 > psw.txt

5. Obfuscator works with "obf_config.cfg" file and accepts code that will be obfuscated through cmd arguments like

   		./obfuscator check.c

### ACCEPTED!
