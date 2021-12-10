.MODEL SMALL
.STACK 200H
.CODE
START:

Mov ah, 2
Mov dl, 1
Int 21h

mov ah, 4ch
mov al,00h
int 21h

END START