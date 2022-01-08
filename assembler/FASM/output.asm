
; Код
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
