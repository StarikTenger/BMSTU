masm

model small

stack 512
assume cs: code, ds: data
data segment
	numbers	dw  34, 45, 56, 67, 75, 89
	diff dw 0
	outstring db "0000000$"
data ends
	
code segment

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

unsigned_diff proc
	; here we use ax & dx
	cmp ax, dx
	jge skip_swap ; if ax < fx we swap 'em
		push ax
		mov ax, dx
		pop dx
	skip_swap:
	sub ax, dx
	ret
unsigned_diff endp

main:
	; black magic :s
	mov ax, data
	mov ds, ax
	
	; reg for diff
	mov diff, 7fffh
	; reg for diffex
	xor cx, cx
	loop_start:
		mov dx, numbers[bx]
		add bx, 2 ; iterator
		mov ax, numbers[bx]
		call unsigned_diff
		
		; comparing
		cmp ax, diff
		jge endcomp
			mov diff, ax
			
			; uncomment following lines to enable output
			;push bx ; save register
			;push cx ; save register
			;call tostring
			;call prntstr
			;pop cx; restore register
			;pop bx; restore register
			
			mov cx, bx ; write index
		endcomp:
		; looping
		cmp bx, 10
		jnge loop_start
	
	; output
	mov ax, cx
	shr ax, 1
	sub ax, 1
	call tostring
	call prntstr

	mov ah, 4ch ; return control to DOS
	int 21h


mov ax,4c00h
int 21h
code ends
end main
