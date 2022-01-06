include macro.inc
assume cs: code, ds: data

data segment
    newline db 0Ah, "$" ; we use this to print newline
    string db 100, 99 dup ('$')
    string1 db 100, 99 dup ('$')
    max_len dw 16
    num_a db 100, 99 dup (0)
data ends

code segment

numtostring macro num, string
    mov ax, max_len
    add ax, 2
    xor si, si ; si for indexing
    xor bx, bx
    loop_numtostring:
        mov ch, num[si]
        add ch, '0'
        mov string[bx], ch

        inc si
        inc bx
        ifless si, ax, break_numtostring
            jmp loop_numtostring
        break_numtostring:
endm

tonum macro string, num
    strlen string, a ; strlen in ax
    mov bx, max_len
    sub bx, ax  ; num_offset in bx
    add ax, 2
    mov si, 2 ; si for indexing
    loop_tonum:
        mov ch, string[si]
        sub ch, '0'
        mov num[bx + 2], ch

        inc si
        inc bx
        ifless si, ax, break_tonum
            jmp loop_tonum
        break_tonum:

    numtostring num, string1
    println string1
endm

start:
    initds

    scanstr string

    tonum string, num_a

    endprogram
code ends
end start