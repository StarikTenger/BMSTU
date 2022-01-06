endprogram macro
    mov ah, 4ch
    int 21h
endm

; print string
print macro str
    push ax
    mov ah, 09h
    lea dx, str
    add dx, 2 ; first 2 chars stand for length
    int 21h
    pop ax
endm

; print string
println macro str
    print str
    ; print newline
    push ax
    mov ah, 09h
    lea dx, newline
    int 21h
    pop ax
endm

tostring macro num, output_string
    push ax
    push bx
    push cx
    push di
    mov ax, num
	mov di, 4 ; Elem diffex
	mov cx, 5 ; Iterations
	MOV BL,10
	mov output_string[5], 10
	mov output_string[6], 13
	goto:
		DIV BL ; Get another digit
		mov output_string[di], ah
		add output_string[di],"0"
		mov ah,0
		sub di,1 ;di=di-1
	loop goto
	
    pop di
    pop cx
    pop bx
    pop ax
endm

ifless macro a, b, endmark
    cmp a, b
    jge endmark
endm

ifequal macro a, b, endmark
    cmp a, b
    je endmark
endm

ifspace macro symbol, endmark
    push ax
    push bx
    mov ah, ' '
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    jne endmark
endm

ifnotspace macro symbol, endmark
    push ax
    push bx
    mov ah, ' '
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    je endmark
endm

ifnotend macro symbol, endmark
    push ax
    push bx
    mov ah, '$'
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    je endmark
endm

ifflag macro flagname, endmark
    push ax
    push bx
    xor ax, ax
    mov bx, flagname
    cmp bx, ax
    pop bx
    pop ax
    je endmark
endm

settrue macro flagname
    mov flagname, 1
endm

setfalse macro flagname
    mov flagname, 0
endm

movesymbol macro s1, s2
    push ax
    mov ah, s2
    mov s1, ah
    pop ax
endm
