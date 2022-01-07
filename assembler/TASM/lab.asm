include macro.inc
assume cs: code, ds: data

data segment
    newline db 0Ah, "$" ; we use this to print newline
    string db 100, 99 dup ('$')
    string1 db 100, 99 dup ('$')
    max_len dw 16
    num_a db 100, 99 dup (0) ; first num
    num_b db 100, 99 dup (0) ; second num
    num_c db 100, 99 dup (0) ; result
    notation db 10 ; decimal or hex

    ; error messages
    error_wrong_symbol db 100, " error: non-numerical symbol $"
data ends

code segment

; hex digit mapping
; working with ch register
tohex proc
    mov cl, 60h
    ifless cl, ch, tohexendif
        sub ch, 'a'
        add ch, ':'
    tohexendif:
    ret
tohex endp

fromhex proc
    mov cl, '9'
    ifless cl, ch, fromhexendif
        sub ch, ':'
        add ch, 'a'
    fromhexendif:
    ret
fromhex endp

numtostring proc
    mov bp, sp
    mov si, [bp + 2] ; num offset in di
    mov ax, max_len
    xor di, di ; di for indexing
    mov bx, 2
    loop_numtostring:
        mov ch, [si]
        add ch, '0'
        ; hex mapping
        call fromhex
        mov string[bx], ch

        inc si
        inc di
        inc bx
        ifless di, ax, break_numtostring
            jmp loop_numtostring
        break_numtostring:
    ret
numtostring endp

printnum macro num
    mov dx, offset num
    push dx
    call numtostring
    println string
endm

tonum proc
    mov bp, sp
    mov di, [bp + 2] ; num offset in di
    strlen string, a ; strlen in ax
    mov bx, max_len
    sub bx, ax  ; num_offset in bx
    add ax, 2
    mov si, 2 ; si for indexing
    xor dx, dx
    mov [di], dx ; fixing first digit
    loop_tonum:
        mov ch, string[si]
        ; hex mapping
        call tohex
        ; checking for number
        ifnotnumber ch, ok_it_is_number
            error_symbol error_wrong_symbol, ch
        ok_it_is_number:

        sub ch, '0'
        mov [di + bx], ch

        inc si
        inc bx
        ifless si, ax, break_tonum
            jmp loop_tonum
        break_tonum:
    ret
tonum endp

scannum macro num
    scanstr string
    mov dx, offset num
    push dx
    call tonum
endm

calculate_sum proc
    mov si, max_len
    sub si, 1
    loop_sum:
        ; put local sum in ch
        xor cx, cx
        mov ah, num_a[si]
        mov bh, num_b[si]
        mov ch, num_c[si]

        add ch, ah
        add ch, bh
        
        ; if overflow
        mov cl, notation
        dec cl
        ifless cl, ch, sum_overflow
            ; reminder in ch
            sub ch, notation
            ; add 1 to next digit
            mov cl, 1
            mov num_c[si - 1], cl
        sum_overflow:

        mov num_c[si], ch

        dec si
        cmp si, 0
        jl break_sum
        jmp loop_sum
    break_sum:
    ret
calculate_sum endp

calculate_diff proc
    mov si, max_len
    sub si, 1
    xor dh, dh
    loop_diff:
        ; put local diff in ch
        xor cx, cx
        mov ah, num_a[si]
        mov bh, num_b[si]

        add ch, ah
        sub ch, bh
        sub ch, dh
        
        ; if overflow
        xor cl, cl ; cl <--- 0
        xor dh, dh ; dh <--- 0
        ifless ch, cl, diff_overflow
            ; reminder in ch
            add ch, notation
            ; add 1 to next digit
            mov dh, 1
            
        diff_overflow:

        mov num_c[si], ch

        dec si
        cmp si, 0
        jl break_diff
        jmp loop_diff
    break_diff:
    ret
calculate_diff endp

calculate_prod proc
    mov di, max_len
    sub di, 1
    xor bx, bx
    
    loop_sumprod:
        mov si, max_len
        sub si, 1
        loop_prod:
           
            xor ax, ax
            xor cx, cx
            xor dx, dx
            mov al, num_a[si]
            mov dl, num_b[di]
            
            ; put local prod in al
            mul dx
            
            ; reminder in ch
            mov cl, notation
            div cl
            ; add to next digit
            sub si, bx
            add num_c[si - 1], al
            add num_c[si], ah
            add si, bx

            dec si
            cmp si, 0
            jl break_prod
            jmp loop_prod
        break_prod:

        inc bx
        dec di
        cmp di, 0
        jl break_sumprod
        jmp loop_sumprod
    break_sumprod:

    mov di, max_len
    sub di, 1
    loop_fix:
        ; reminder in ch
        xor ax, ax
        mov cl, notation
        mov al, num_c[di]
        div cl
        ; add to next digit
        add num_c[di - 1], al
        mov num_c[di], ah

        dec di
        cmp di, 0
        jl break_fix
        jmp loop_fix
    break_fix:
    ret
calculate_prod endp

start:
    initds

    scannum num_a
    scannum num_b
    xor dx, dx
    mov num_c[0], dh ; fixing first digit of num_c

    call calculate_prod

    printnum num_a
    printnum num_b
    printnum num_c
    
    endprogram
code ends
end start