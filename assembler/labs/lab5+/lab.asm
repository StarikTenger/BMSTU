include 'macro.inc'

use16
org 100h                   
    jmp start   

; DATA
newline db 0Ah, "$" ; we use this to print newline
string db 100, 99 dup ('$')
string_reversed db 100, 99 dup ('$')
words dw 100, 99 dup (0)
space db 0Ah, ' '
flag dw 0
wordcount dw 0

include 'inout.asm'

scanstr:
    ; considering that sting lies in dx
    ; input string
    xor ax, ax
    mov ah, 0Ah
    int 21h
    
    ; fix string adding '$' to it's end
    mov si, dx
    xor bh, bh
    mov bl, [si+1]
    mov ch, '$'
    add bx, 2
    mov [si+bx], ch

    ; print newline
    mov dx, newline
    mov ah, 09h
    int 21h
    ret

reverse:
    ; step 1: finding words
    xor si, si
    settrue flag

    loop1:
        ifflag flag, endif1 ; detecting start of word
        ifnotspace [string + si], endif1
            inc [wordcount]
            push si
            setfalse flag
        endif1:

        ifspace [string + si], endif2
            settrue flag
        endif2:

        inc si
        inc [size]
        ifnotend [string + si], breakmark1 ; break loop if we reach '$'
        jmp loop1
    breakmark1:

    ; step 2: put words in new string
    xor si, si ; iterate through string
    xor di, di ; iterate through word list
    loop3:
        pop bx
        loopword:
            movesymbol [string_reversed + si], [string + bx]
            
            inc bx
            inc si
            ifnotspace [string + bx], breakword
            ifnotend [string + bx], breakword
                jmp loopword
            breakword:

        ; add space
        movesymbol [string_reversed + si], ' '
        inc si

        inc di
        ifless di, [wordcount], endifless
            jmp loop3
        endifless:
    ret

start:
    ; read strings
    ;mov dx, string
    ;call scanstr

    call read_from_file

    call reverse

    call write_to_file

    endprogram
