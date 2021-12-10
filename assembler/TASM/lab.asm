masm

model small

stack 512
assume cs: code, ds: data
data segment
	numbers	dw  34, 45, 56, 67, 75, 89
	ind dw 0
	outstring db "0000000$"
data ends
	
code segment

tostring proc
	mov si,4 ; Индекс элемента
	mov cx, 5 ;итерации цикла	
	MOV BL,10
	mov outstring[5], 10
	mov outstring[6], 13
	goto:
		DIV BL ;Получаем очередную цифру (делим на 10)
		mov outstring[si], ah
		add outstring[si],"0" ;Для вывода цифры
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

unsigned_indff proc
	; here we use ax & dx
	cmp ax, dx
	jge skip_swap
		push ax
		mov ax, dx
		pop dx
	skip_swap:
	sub ax, dx
	ret
unsigned_indff endp

main:
	; black magic ((()))
	mov ax, data
	mov ds, ax
	xor bx, bx
	; reg for indff
	mov ind, 07fffh
	; reg for index
	xor cx, cx
	loop_start:
		mov dx, numbers[bx]
		add bx, 2 ; iterator
		mov ax, numbers[bx]
		call unsigned_indff
		
		
		
		; comparing
		cmp ax, ind
		jge endcomp
			mov ind, ax
			
			; output
			push bx ; save register
			push cx ; save register
			call tostring
			call prntstr
			pop cx; restore register
			pop bx; restore register
			
			mov cx, bx ; write index
		endcomp:
		; looping
		cmp bx, 10
		jnge loop_start
	
	; oudbud
	mov ax, cx
	shr ax, 1
	sub ax, 1
	call tostring
	call prntstr

	mov ah, 4ch                  ; return control to DOS
	int 21h


mov ax,4c00h
int 21h
code ends
end main
