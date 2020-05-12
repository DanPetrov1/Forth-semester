.model small
.stack 100h
.data

max     db 201 
len     db ?
buffer  db 201 dup(?) 
maxBorder dw 2 dup(?) 
minBorder dw 2 dup(?) 

newl     EQU 0Ah, 0Dh
MAX_EL   dw  10

counter dw 2 dup(?)

matrix  dw 10 dup(?)
resultCount db "Result count:",10,13,'$'
message db "Max mul:",10,13,'$'
messageEnd db "Row number:",10,13,'$'
error db "Error, repeat input,please",10,13,'$'
overflow db "Overflow",10,13 , '$'
inputMax db "Write max border:",10,13,'$'
inputMin db "Write min border:",10,13,'$'

.code
    jmp start
    errorMessage proc near
    push ax
    push bx
    push cx
    push dx
    mov ah,09h
    mov dx, offset error
    int 21h
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp

    printNumber proc near; ax - number
    push ax; number
    push bx; base
    push cx; counter
    push dx; remainder
    
    xor cx, cx
    mov bx, 10
    
    cmp ax , 0
    jge printNumber_parse
    push ax
    
    mov ah , 02h
    mov dl , '-'
    int 21h
    
    pop ax
    neg ax ;cменить знак
    
    
    printNumber_parse:
    xor dx, dx
    idiv bx
    add dl, '0'
    push dx
    inc cx
    cmp ax, 0
    jne printNumber_parse
    mov ah, 02h
    
    printNumber_loop:
    pop dx
    int 21h
    loop printNumber_loop
    
    printNumberExit:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
endp
convertNumber proc near
    push si
    push cx
    push bx
    push dx
    
    checkIsEmpty:
    
    push ax
    
    mov al , [len] 
    cmp al , 0
    je endFirstConvert
    
    pop ax
    
    convertNumberStart:
    mov si, offset buffer
    mov al , [buffer]
    xor cx, cx; base
    xor ax, ax; result
    xor bx, bx;  
    mov cx, 10
    mov dx , 1
    lodsb ;mov al,[si] inc si
    cmp al , '-'
    jne nonMinus
    mov dx , -1
    push dx
    xor ax , ax
    jmp convertNumberLoop
    
    nonMinus:
    mov dx , 1
    push dx
    cmp al, '$'
    je endConvert
    cmp al, '0'
    jb endFirstConvert
    cmp al, '9'
    ja endFirstConvert
    sub al, '0' 
    
    convertNumberLoop:
    push ax
    lodsb
    mov bl, al
    pop ax
    cmp bl, '$'
    je endConvert
    cmp bl, '0'
    jb endFirstConvert
    cmp bl, '9'
    ja endFirstConvert
    sub bl, '0'
    xor dx , dx
    imul cx
    cmp dx , 0
    jne overflowL
    
    add ax, bx
    
    jmp convertNumberLoop
    
    endFirstConvert:
    call errorMessage
    ;-----------------------------
    lea bx , buffer
    call input
    pop dx
    jmp checkIsEmpty
    ;---------------------------------
    jmp convertNumberLoop
    endConvert:
    
    test ax , 8000h
    jz nonOverfolw
    
    overflowL:
    
    mov ah , 09h
    lea dx , overflow
    int 21h
    
    jmp endFirstConvert
    
    
    nonOverfolw:
    
    pop dx
    mul dx
    pop dx
    pop bx
    pop cx
    pop si
    ret    
endp


input proc near
    push ax
    push bx
    push cx
    push dx
    mov ah, 0ah
    xor al, al
    lea dx, max
    int 21h
    xor ah, ah
    mov al, len
    mov si, ax
    mov buffer[si], '$'
    dec si
    mov ah, 2
    mov dl, 10
    int 21h
    mov dl, 13
    int 21h 
    pop dx
    pop cx
    pop bx
    pop ax
ret
endp


start:
    mov ax, @data  ;Writes correct data into ds and es
    mov ds, ax     ;Important for multi-segment programs
    
    mov es, ax
    xor ax,ax
    xor cx, cx
    xor dx,dx
    mov ax,[MAX_EL]
    push ax
    mov  cx, ax
    xor ax,ax
    xor di,di
    mov bx, offset matrix
    
    cycle:
    
    call input
    call convertNumber
    
    mov [matrix+di], ax
    add di , 2
    loop cycle

    xor cx, cx
    xor di, di
    xor ax, ax
    xor bx, bx
   
    pop ax
    mov cx, ax
    xor ax,ax
   
    p1:
    mov ax, [matrix + di]
    call printNumber
    mov ah, 2
    mov dl, ' '
    int 21h
   
    add di , 2
    loop p1
   
    mov ah, 2
    mov dl, 10
    int 21h
    mov dl, 13
    int 21h 
    
   
    mov dx, offset inputMax
    mov ah, 9
    int 21h
    
    call input
    
    call convertNumber
    
    mov [maxBorder], ax
    
    
    mov dx, offset inputMin
    mov ah, 9
    int 21h
    
    call input
    
    call convertNumber
    
    mov [minBorder], ax
    
    xor cx,cx
    mov [counter], cx
    
    mov bx, [maxBorder]
    cmp [minBorder], bx
    jg exitAll
    
    xor si, si
    mov cx,[MAX_EL]
    mainCycle:
    mov dx, [minBorder]
    cmp [matrix + si], dx
    jl repeate
    mov dx,  [maxBorder]
    cmp [matrix + si], dx
    jg repeate
    mov dx, [counter]
    inc dx
    mov [counter], dx
    repeate:
    inc si
    inc si
    loop mainCycle
    
    
 exitAll:
  
    mov dx, offset resultCount
    mov ah, 9
    int 21h
    
    mov ax, [counter]
    call printNumber
    
    mov ax , 4C00h
    int 21h
end start
