// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.
	@i // i = 0
	M=0
	
	@color
	M=0
	
	@SCREEN
	D=A
	@addr
	M=D

(LOOP)
	@i
	M=0
	@KBD
	D=M  // D = RAM[24576]
	@BLACK
	D;JGT  // if RAM[kbaddress] > 0 GOTO BLACK else GOTO WHITE
	@WHITE
	0;JMP

(BLACK)
	@color
	M=-1
		
	@SETCOLOR
	0;JMP
	
(WHITE)
	@color
	M=0
		
	@SETCOLOR
	0;JMP
	
	
(SETCOLOR)
	@i  // if i >= 8192 goto LOOP ELSE SETCOLOR
	D=M
	@8192
	D=D-A
	@LOOP
	D;JGE
	
	@addr // set RAM[screenaddr] = -1
	D=M
	@i
	D=D+M // screenaddr = screenaddr + i
	@R0
	M=D
	@color
	D=M
	@R0
	A=M
	M=D
	
	@i
	M=M+1
		
	@SETCOLOR
	0;JMP