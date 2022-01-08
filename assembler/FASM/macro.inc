macro endprogram {
    mov ah, 4ch
    int 21h
}

; print string
macro print str* {
    push ax
    mov ah, 09h
    mov dx, str
    add dx, 2 ; first 2 chars stand for length
    int 21h
    pop ax
}

; print string
macro println str* {
    print str
    ; print newline
    push ax
    mov ah, 09h
    mov dx, newline
    int 21h
    pop ax
}

macro printchar char* {
    push ax
    push dx
    mov ah, 2
    mov dl, char
    int 21h
    pop dx
    pop ax
     ; print newline
    push ax
    mov ah, 09h
    mov dx, newline
    int 21h
    pop ax
}

macro printdigit digit* {
    push dx
    mov dx, digit
    add dl, '0'
    printchar dl
    sub dl, '0'
    pop dx
}

macro ifless a*, b*, endmark* {
    cmp a, b
    jge endmark
}

macro ifequal a*, b*, endmark* {
    cmp a, b
    je endmark
}

macro ifspace symbol*, endmark* {
    push ax
    push bx
    mov ah, ' '
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    jne endmark
}

macro ifnotspace symbol*, endmark* {
    push ax
    push bx
    mov ah, ' '
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    je endmark
}

macro ifnotend symbol*, endmark* {
    push ax
    push bx
    mov ah, '$'
    mov bh, symbol
    cmp bh, ah
    pop bx
    pop ax
    je endmark
}

macro ifflag flagname*, endmark* {
    push ax
    push bx
    xor ax, ax
    mov bx, [flagname]
    cmp bx, ax
    pop bx
    pop ax
    je endmark
}

macro settrue flagname* {
    mov [flagname], 1
}

macro setfalse flagname* {
    mov [flagname], 0
}

macro movesymbol s1*, s2* {
    push ax
    mov ah, s2
    mov s1, ah
    pop ax
}
