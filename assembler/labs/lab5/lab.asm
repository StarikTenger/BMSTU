include macro.inc
assume cs: code, ds: data

data segment
    newline db 0Ah, "$" ; we use this to print newline
    string db 100, 99 dup ('$')
    string_reversed db 100, 99 dup ('$')
    words dw 100, 99 dup (0)
    space db 0Ah, ' '
    flag dw 0
    wordcount dw 0
data ends

; init data segment
initds macro
    mov ax, data
    mov ds, ax
endm

code segment	

scanstr proc
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
    mov dx, offset newline
    mov ah, 09h
    int 21h
    ret
scanstr endp

reverse proc
    ; step 1: finding words
    mov si, 2
    settrue flag
    loop1:
        ifflag flag, endif1 ; detecting start of word
        ifnotspace string[si], endif1
            ; tostring si, string_reversed
            ; println string_reversed
            inc wordcount
            push si
            setfalse flag
        endif1:

        ifspace string[si], endif2
            settrue flag
        endif2:

        inc si
        ifnotend string[si], breakmark1 ; break loop if we reach '$'
        jmp loop1
    breakmark1:

    ; step 2: put words in new string
    mov si, 2 ; iterate through string
    xor di, di ; iterate through word list
    loop3:
        pop bx
        loopword:
            movesymbol string_reversed[si], string[bx]
            
            inc bx
            inc si
            ifnotspace string[bx], breakword
            ifnotend string[bx], breakword
                jmp loopword
            breakword:

        ; add space
        movesymbol string_reversed[si], ' '
        inc si

        inc di
        ifless di, wordcount, endifless
            jmp loop3
        endifless:
    ret
reverse endp

start:
    initds

    ; read strings
    mov dx, offset string
    call scanstr

    call reverse
    println string_reversed

    endprogram
code ends
end start