; Данные
file_name db 'input.txt',0
s_error   db 'Error!',13,10,'$'
s_file    db '----[ file "Hello.txt" ]$'
endline   db 13,10,'$'
s_pak     db 'Press any key...$'
buffer    rb 81             ;80 + 1 байт для символа конца строки '$'
handle    rw 1              ;Дескриптор файла
; Данные
output_file_name db 'output.txt',0
size      db 0
;-------------------------------------------------------------------------------

write_to_file:
    mov ah,3Ch              ;Функция DOS 3Ch (создание файла)
    mov dx, output_file_name        ;Имя файла
    xor cx,cx               ;Нет атрибутов - обычный файл
    int 21h                 ;Обращение к функции DOS
    jnc @F                  ;Если нет ошибки, то продолжаем
    call error_msg          ;Иначе вывод сообщения об ошибке
    jmp exit                ;Выход из программы
@@: mov [handle],ax         ;Сохранение дескриптора файла
 
    mov bx,ax               ;Дескриптор файла
    mov ah,40h              ;Функция DOS 40h (запись в файл)
    mov dx,string_reversed           ;Адрес буфера с данными
    movzx cx,[size]         ;Размер данных
    int 21h                 ;Обращение к функции DOS
    ret
read_from_file:
    mov ah,3Dh              ;Функция DOS 3Dh (открытие файла)
    xor al,al               ;Режим открытия - только чтение
    mov dx,file_name        ;Имя файла
    xor cx,cx               ;Нет атрибутов - обычный файл
    int 21h                 ;Обращение к функции DOS
    jnc @F                  ;Если нет ошибки, то продолжаем
    call error_msg          ;Иначе вывод сообщения об ошибке
    jmp exit                ;Выход из программы
@@: mov [handle],ax         ;Сохранение дескриптора файла

    mov bx,ax               ;Дескриптор файла
    mov ah,3Fh              ;Функция DOS 3Fh (чтение из файла)
    mov dx,string           ;Адрес буфера для данных
    mov cx,80               ;Максимальное кол-во читаемых байтов
    int 21h                 ;Обращение к функции DOS
    jnc @F                  ;Если нет ошибки, то продолжаем
    call error_msg          ;Вывод сообщения об ошибке
    jmp close_file          ;Закрыть файл и выйти из программы

@@: mov bx,string
    add bx,ax               ;В AX количество прочитанных байтов
    mov byte[bx],'$'        ;Добавление символа '$'
    mov ah,9
    mov dx,string
    int 21h                 ;Вывод содержимого файла
    mov dx,endline
    int 21h                 ;Вывод перехода на новую строку
    ret

    mov cx,80
    call line               ;Вывод линии

close_file:
    mov ah,3Eh              ;Функция DOS 3Eh (закрытие файла)
    mov bx,[handle]         ;Дескриптор
    int 21h                 ;Обращение к функции DOS

exit:
    mov ah,9
    mov dx,s_pak
    int 21h                 ;Вывод строки 'Press any key...'
    mov ah,8                ;\
    int 21h                 ;/ Ввод символа без эха
    mov ax,4C00h            ;\
    int 21h                 ;/ Завершение программы

;-------------------------------------------------------------------------------
; Процедура вывода сообщения об ошибке
error_msg:
    mov ah,9
    mov dx,s_error
    int 21h                 ;Вывод сообщения об ошибке
    ret
;-------------------------------------------------------------------------------
; Вывод линии
; CX - количество символов
line:
    mov ah,2                ;Функция DOS 02h (вывод символа)
    mov dl,'-'              ;Символ
@@: int 21h                 ;Обращение к функции DOS
    loop @B                 ;Команда цикла
    ret
