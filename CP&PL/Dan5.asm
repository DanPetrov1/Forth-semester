model small
.386
.stack 100h
.data
    fname db 256 dup(0)
    bufNumber db 256 dup(0)
    wordNum dw 3
    fd dw 0
    readPos dd 0
    
    chunkSize equ 256
    chunk db chunkSize dup('$')
    chunkLast dw 0
    
    stringNum dd 0 ;4 байта
    stringLen dd 0
    
    errorFileMessage db 'File is not open!$'
    errorArgsMessage db 'Wrong arguments', 10, 13, 'Input: file.txt and number$'
    errorNumMessage  db 'Word number should [0, 32767]$'
    doneMessage    db 'All done! All ok!$'
    
    base dw 10
    endl db 10, 13, '$'
.code

atoi proc  
    push bp
    mov bp, sp   
    pusha        
        
    ;[ss:bp+4+0] - адрес числа  
    ;[ss:bp+4+2] - адрес строки
    ;[ss:bp+4+4] - ошибка, если 1
    mov di, [ss:bp+4+2]  
        
    xor bx, bx     
    xor ax, ax   
    xor cx, cx
    xor dx, dx
        
    cmp BYTE PTR [di + bx], '-'
    jne atoi_loop
        
    inc cx; set negative after loop  
    inc bx
            
    ;парсить до ошибки
    atoi_loop:    
            
    cmp BYTE PTR [di + bx], '0'    ;проверка на то, является ли числом вводимое значение
    jb atoi_error 
    cmp BYTE PTR [di + bx], '9'    
    ja atoi_error
                                
    mul base ;умножаем на 10
    mov dh, 0
    mov dl, [di + bx] 
    sub dl, '0'  ;преващаем в число, отнимая ascii код 0
    add ax, dx  ;добавляем к основному числу
    jo atoi_error  ;если был поднят флаг переполнения, то выводим ошибку   
        
    inc bx 
    cmp BYTE PTR [di + bx], 0 ;если переполнения не было и строка не закончилась, то продолжаем, пока не конец
    jne atoi_loop  
        
    jmp atoi_result 
        
    atoi_error: ;ошибка, выход и процедуры
    mov BYTE PTR [ss:bp+4+4], 1    
    jmp atoi_end 
        
    atoi_result:
    mov BYTE PTR [ss:bp+4+4], 0  ;без ошибок
    cmp cx, 1 ;проверяем, был ли минус перед числом
    jne atoi_end ;если нет, то завершаем программу
    neg ax ; если нет, то меняем знак на противоположный
        
    atoi_end: 
    mov di, [ss:bp+4+0]
    mov [di], ax ;помещаем число по заданному адресу
    
    popa ;завершаем процедуру
    pop bp
    ret 
endp 

print_str proc      
    push bp
    mov bp, sp   
    pusha 
        
    mov dx, [ss:bp+4+0]     
    mov ax, 0900h
    int 21h 
        
    mov dx, offset endl ;выводим новую строку
    mov ax, 0900h
    int 21h  
        
    popa
    pop bp      
    ret  
endp

printNumberTest proc near; ax - number
    push ax; number
    push bx; base
    push cx; counter
    push dx; remainder
    
    xor cx, cx
    mov bx, 10
    
    cmp ax , 0
    jge printNumber_parseA
    push ax
    
    mov ah , 02h
    mov dl , '-'
    int 21h
    
    pop ax
    neg ax ;cменить знак
        
    printNumber_parseA:
    xor dx, dx
    idiv bx
    add dl, '0'
    push dx
    inc cx
    cmp ax, 0
    jne printNumber_parseA
    mov ah, 02h
    
    printNumber_loopA:
    pop dx
    int 21h
    loop printNumber_loopA
    
    printNumberExitA:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

    start:
    mov ax, @data ; загрузка сегмента данных
    mov ds, ax
    
    mov bl, es:[80h] ;args line length 
    add bx, 80h      ;args line last    
    mov si, 82h      ;args line start
    mov di, offset fname 
    
    cmp si, bx ;если начало < конца кс
    ja bad_arguments ;если неверные аргументы, то переходим по меке bad_arguments
    ;если нет, то продолжаем работать с введёнными в кс параметрами
    parse_path:
    
    cmp BYTE PTR es:[si], ' ' ;провнряем, не пробел ли
    je parsed_path 
              
    mov al, es:[si] ;помещаем из командной строки имя файла в переменную fileName
    mov [di], al      
              
    inc di
    inc si
    cmp si, bx ;если всё хорошо, то продолжаем работать
    jbe parse_path
    
    parsed_path:  
    mov di, offset bufNumber  
    inc si
    cmp si, bx;если начало < конца кс
    ja bad_arguments  ;если неверные аргументы, то переходим по меке bad_arguments
    ;если нет, то продолжаем работать с введёнными в кс параметрами
     
    parse_number:
     
    cmp BYTE PTR es:[si], ' ' ;провнряем, не пробел ли
    je parsed_number 
              
    mov al, es:[si] ;помещаем из командной строки данные в bufNumber 
    mov [di], al      
              
    inc di
    inc si
    cmp si, bx;если всё хорошо, то продолжаем работать
    jbe parse_number
    
    parsed_number:
    push 0
    mov di, offset bufNumber 
    push di
    mov di, offset wordNum 
    push di
    call atoi
    pop ax    
    pop ax 
    pop ax;error

    cmp ax, 1 ;если ошибка
    je bad_number

    cmp wordNum, 0
    jl bad_number

    call process_file

    mov ax, offset doneMessage;смещение строки done_msg
    push ax ; сохраняем значение в стеке
    call print_str  ; функция вывода сообщения
    pop ax ;достаём из стека ax

    exit: ;выход из программы
    mov ax, 4C00h
    int 21h

    bad_file:
    mov ax, offset errorFileMessage ;смещение строки errorFileMessage
    push ax ; сохраняем значение в стеке
    call print_str ; функция вывода сообщения
    pop ax ;достаём из стека ax
    jmp exit ;завершаем программу

    bad_arguments:
    mov ax, offset errorArgsMessage ;смещение строкиerrorArgsMessage 
    push ax ; сохраняем значение в стеке
    call print_str  ; функция вывода сообщения
    pop ax ;достаём из стека ax
    jmp exit  ;завершаем программу

    bad_number:
    mov ax, offset errorNumMessage ;смещение строкиerrorArgsMessage 
    push ax  ; сохраняем значение в стеке
    call print_str  ; функция вывода сообщения
    pop ax ;достаём из стека ax
    jmp exit ;завершаем программу

    process_file:
    ;открыть существующий файл
    mov dx, offset fname; адрес строки с полным именем файла
    mov ah, 3Dh
    mov al, 02h ;режим доступа(и для чтениия, и для записи)
    int 21h
    mov fd, ax ;в ax-код ошибки или дескриптор файла если всё хорошо

    mov bx, ax ;помещаем код ошибки в  bx
    jnc read_file_chunk ;переход, если перенос не установлен
    jmp bad_file;если ошибка произошла и перенос установлен, то файл не открыт, переходим по метке и сообщаем об этом

    read_file_chunk:  
    mov ah, 42h ; переместить указатель чтения-записи
    mov cx, WORD PTR [offset readPos] ; cx:dx- расстояние, на которое надо переместить указатель-знаковое число
    mov dx, WORD PTR [offset readPos + 2]
    mov al, 0 ; перемещение относительно начала файла
    mov bx, fd ;файловый дескрипрор
    int 21h
        
    mov cx, 1 ;число байт для чтения
    mov dx, offset chunk ;адрес буфера для приема данных
    mov ah, 3Fh ;чтение из файла или устройства
    mov bx, fd ;идентификатор файла,
    int 21h
    jc close_file ;если флаг CF был поднят, то ошибка
    ;если операция выполнена успешно, то в ax число считанных байт
    cmp ax, 0 ;сравниваем с 0 cимволом
    je close_file ;если файл пуст, то переходим по метке close_file и закрываем его
        
    cmp [chunk],13
    je nextIteration
    cmp [chunk],0
    je close_file
    
    mov ebx,[stringLen]
    inc ebx
    mov [stringLen],ebx
    
    mov cx, WORD PTR [offset readPos] ;если файл не пустой или мы что-то смогли прочитать то запоминаем позицию
    mov dx, WORD PTR [offset readPos + 2]
    add dx, ax; добавляем к смещению число считанных байт
    adc cx, 0 ;Сложение с переносом
    mov WORD PTR [offset readPos], cx ;новое смещение чтения-записи
    mov WORD PTR [offset readPos + 2], dx

    ; mov chunkLast, ax;помещаем число считанных байт
    ; call process_chunk

    jmp read_file_chunk
        
    nextIteration:
    mov ebx, [stringLen]
    dec ebx
    mov dx,[wordNum]
    cmp ebx, edx
    jb incCount
    jmp nextCount
    incCount:
    mov ebx,[stringNum]
    inc ebx
    mov [stringNum],ebx
    nextCount:
    xor ebx,ebx
    mov [stringLen],ebx
        
    mov cx, WORD PTR [offset readPos] ;если файл не пустой или мы что-то смогли прочитать то запоминаем позицию
    mov dx, WORD PTR [offset readPos + 2]
    add dx, ax; добавляем к смещению число считанных байт
    adc cx, 0 ;Сложение с переносом
    mov WORD PTR [offset readPos], cx ;новое смещение чтения-записи
    mov WORD PTR [offset readPos + 2], dx
    jmp read_file_chunk
        
    close_file:
        
    mov eax,[stringNum]
    call printNumberTest

    mov ah, 3Eh ;функция закрытия файла
    mov bx, fd ;файловый дескриптор
    int 21h

    mov ax, 4C00h
    int 21h
end start
