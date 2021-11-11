masm

model small

stack 512
assume CS: code, DS: data
data segment

    a db 5

    b db 3

    c db 4

    d db 2
data ends

code segment

main:
	mov AX, data
	mov DS, AX
	xor ax, ax
	xor bx, bx
	xor dx, dx

	mov al, a
	mov bl, b 
	mul bl

	mov dx, ax
	xor ax, ax
	xor bx, bx

	mov al, c
	mov bl, d
	div bl

	xor bx, bx
	mov bx, dx
	xor dx, dx

	add ax, bx
	xor bx, bx
	mov bx, 5
	add ax, bx

	xor bx, bx
	
	; output
	ADD AL, 30H                  ; convert ASCII to DECIMAL code
	
	MOV AH, 2                    ; display the character
	MOV DL, AL     
	INT 21H

	MOV AH, 4CH                  ; return control to DOS
	INT 21H


mov ax,4c00h
int 21h
code ends
end main
