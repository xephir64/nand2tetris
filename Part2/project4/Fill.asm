@KBD
D=A
@addrmax
M=D
@i
M=0
@color
M=0

(MAIN)
    @SCREEN
    D=A
    @i
    M=D
    @KBD
    D=M
    @FILL_SCREEN
    D;JGT
    @CLEAR_SCREEN

(CLEAR_SCREEN)
    @color
    M=0
    @SETCOLOR
    0;JMP

(FILL_SCREEN)
    @color
    M=-1
    @SETCOLOR
    0;JMP

(SETCOLOR)
    @i
    D=M
    @addrmax
    D=M-D
    @MAIN
    D;JEQ
    @color
    D=M
    @i
    A=M
    M=D
    @i
    M=M+1
    @SETCOLOR
    0;JMP