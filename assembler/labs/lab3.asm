assume cs: code, ds: data

data segment
    newline db 0Ah, "$" ; we use this to print newline
    string_source db 100, 99 dup ('$')
    string_symbols db 100, 99 dup ('$')
    outstring db "0000000$" ; string used to do output
    counter dw 0
data ends

code segment	

strspn proc
    ; strings lie here
    push bp
    mov bp, sp
    mov ax, [bp+6]
    mov cx, [bp+4]
    
    ; for indexing
    mov si, ax
    inc si
    add cx,2
    ; nullify counter
    mov counter, 0
    ; LOOP!
    loop_source:
    ; Increment
    inc si
    mov dl, [si]
    cmp dl, '$'
    je exit1
        mov di, cx
        loop_symbols:
        ; Increment
        inc di
        mov dh, [di]
        cmp dh, '$'
        je match_found
            ; Compare symbols
            cmp dl, dh
            jne match_not_found
                inc counter
            jmp match_found

        ; Loop cond
        match_not_found:
        jmp loop_symbols
    match_found:
    ; Loop cond
    
    jmp loop_source
    exit1:
    ; Push result to stack
    ;push counter
    mov di,counter
    mov [bp+4],di
    mov sp,bp
    pop bp
    ret
strspn endp

scanstr proc
    ; considering that sting lies in dx
    ; input string
    xor ax, ax
    mov ah, 0Ah
    int 21h
    
    ; fix string adding '$' to it's end
    mov si,dx
    xor bh, bh  
    mov bl, [si+1]
    mov ch, '$'
    add bx, 2
    mov [si+bx], ch

    ; print newline
    mov dx, offset newline
    mov ah, 09h
    int 21h
    ret
scanstr endp

tostring proc
	mov si,4 ; Elem diffex
	mov cx, 5 ; Iterations
	MOV BL,10
	mov outstring[5], 10
	mov outstring[6], 13
	goto:
		DIV BL ; Get another digit
		mov outstring[si], ah
		add outstring[si],"0"
		mov ah,0
		sub si,1 ;si=si-1
	loop goto
	mov ax,0000h
	ret
tostring endp

prntstr proc
	mov ax, data
	mov ds, ax
	mov ah, 09h
	mov dx, offset outstring
	int 21h
	mov ax,0000h
	ret
prntstr endp

start:
    ; black magic :s
    mov ax, data
    mov ds, ax

    ; read strings
    mov dx, offset string_source
    call scanstr
    mov dx, offset string_symbols
    call scanstr
    
    ; calc strspn
    mov dx,offset string_source
    push dx
    mov dx,offset string_symbols
    push dx
    call strspn

    ; output
    pop ax
    call tostring
    call prntstr

    ; mov dx, offset string_source
    ; add dx, 2

    ; mov ah, 09h
    ; int 21h

    mov ah, 4ch
    int 21h
code ends
end start