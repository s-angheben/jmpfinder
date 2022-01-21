jmpfinder
=========

Jmpfinder is a C/C++ program to find the indirect jump or indirect call instructions of a ELF executable, both aligned and unaligned with respect to the binary's normal instruction stream. The code is heavily influenced by the book: "Practical Binary Analysis: Build Your Own Linux Tools for Binary Instrumentation, Analysis, and Disassembly"

How to build
------------

For the compilation these libraries are required:

* -lbfd		    (`apt install binutils-dev`)
* -lcapstone	    (`apt install libcapstone-dev`)

and then `make`

How to use
----------

Jmpfinder works only with x86_64 target program. 
The target program must have the table symbols (not stripped) because for instruction searching, jmpfinder need the address of the .text section and his size.

**example:**

	$ ./jmpfinder /bin/ls
	call qword ptr [rax + 0x100f41f3]	[ 0xed82 ]
	call qword ptr [rax + 0x19b60f41]	[ 0x13252 ]
	call qword ptr [rax + 0x1b8]	[ 0xb952 0xbf02 ]
	call qword ptr [rax + 0x20478b48]	[ 0x16b72 ]
	call qword ptr [rax + 0x247c8348]	[ 0xd3c2 ]
	call qword ptr [rax + 0x38478b48]	[ 0x160e2 ]
	call qword ptr [rax + 0x3f07c641]	[ 0x7ea2 ]
	call qword ptr [rax + 0x42247c80]	[ 0x13302 ]
	call qword ptr [rax + 0x43247c80]	[ 0x13562 ]
	call qword ptr [rax + 0x48555441]	[ 0x11112 ]
	call qword ptr [rax + 0x48ec8348]	[ 0x145e2 ]
	call qword ptr [rax + 0x48f08948]	[ 0xff52 ]







