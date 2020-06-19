.model tiny
.286
.code
org 100h
start:
    jmp installer
    
    dollar equ 36
    max_hours equ 23
    max_minutes equ 59
    space_sym equ 32 
    null_sym equ 48
    nine_sym equ 57
    command_line_error db "Illegal time or sound length!$"
    buffer_1 db 20
    len_1 db 0
    command_line db 19 dup("$")
    buffer_2 db 7
    len_2 db 0
    n db 6
    error1 db "Illegal command line!$"
    error2 db "Illegal number of hours!$"
    error3 db "Illegal number of minuets!$"
    yes db " Alarm clock!" 
    hours db 0
    minutes db 0
    sound_length dw 0
    old_interrupt dd 0
    is_resident db 0
    
    ;---------------------
enable_sound proc near          
    push ax
    push bx
    push dx
    mov bx,ax
    in al,61h
    or al,3
    out 61h,al
    mov al,00001011b
    pop dx
    pop bx
    pop ax
    ret    
enable_sound endp
;---------------------
disable_sound proc near
    push ax
    in al,61h
    mov al,0
    out 61h,al
    pop ax
    ret
disable_sound endp
;---------------------
clear proc near
    mov ah,0
    mov al,3
    int 10h  
    mov ah,02
    mov dh,0
    mov dl,0
    int 10h
    ret
clear endp
;---------------------
cout macro str
    mov ah,09h
    lea dx,str
    int 21h       
endm
;---------------------
atoi proc near
    xor di,di
    xor ax,ax
    xor bx,bx
    xor dx,dx
    cycl:
    mov al,buffer_2[di]
    inc di     
    cmp al,36
    je yw
    push ax
    mov ax,bx
    mov bx,10
    mul bx
    mov bx,ax
    pop ax
    sub al,48
    add bl,al
    jmp cycl
    yw:
    ret
atoi endp
;---------------------
 alarm_clock proc far
        push ds
        push cs
        pop ds

        pusha
        
        pushf                 
        call dword ptr cs:old_interrupt        
        mov al,11110000b     
        out 42h,al
        mov al,00000101b
        out 42h,al
          
        ;duration of the sound
        call enable_sound
        mov bx,sound_length
        timer_cycle:
        xor cx,cx
        mov dx,500
        mov ah,86h
        int 15h
        dec bx
        cmp bx,0
        jne timer_cycle
        call disable_sound 
        
        mov ah,03h
        mov bh,0
        int 10h 
        xor si,si
        mov dl,-1
        cout_cycle:
            mov ah,0ah
            mov al,yes[si]
            mov cx,1
            int 10h
            inc si
            mov ah,02h
            mov bh,0
            inc dl
            int 10h
        cmp si,12
        jne cout_cycle
        
        popa
        pop ds       
        iret     
    alarm_clock endp
 
 installer:
    
    ;command line input    
    xor cx,cx
    xor di,di
    mov si,80h
    command_line_input:
        mov al,es:[si]
        inc si
        cmp al,0                 
        je command_line_end        
        mov buffer_1[di],al
        inc di
        jmp command_line_input
    command_line_end:
    xor si,si
    xor di,di
    xor si,si
    loop_end_e:               
        mov al,buffer_1[si]
        cmp al,13
        je cn1
        inc si
        jmp loop_end_e
    cn1:
    mov buffer_1[si],dollar
    
    ;reading command line
    xor di,di
    xor si,si
    mov dx,4
    hour: 
        mov al,command_line[si]        
        inc si    
        cmp al,space_sym
        je hour_end
        cmp dx,0
        je jump2
        cmp al,null_sym
        jb jump2
        cmp al,nine_sym
        ja jump2
        mov buffer_2[di],al
        inc di    
        mov buffer_2[di],dollar
        dec dx
    jmp hour  
    hour_end:
        
    call atoi
    cmp bx,max_hours
    ja jump4        
    mov hours,bl
                                                                                              
        xor di,di
        mov dx,4
        mov al,command_line[si]
        cmp al,space_sym
        je jump2 
    minute: 
        mov al,command_line[si]        
        inc si    

        jmp jump1
        jump2:
        jmp error1_exit
        jump1:
        jmp jump3
        jump4:
        jmp error2_exit
        jump3: 
         
        cmp al,space_sym
        je minute_end
        cmp dx,0
        je jump2
        cmp al,null_sym
        jb jump2
        cmp al,nine_sym
        ja jump2
        mov buffer_2[di],al
        inc di    
        mov buffer_2[di],dollar
        dec dx
    jmp minute  
    minute_end:

    jmp jump5
    jump6:
    jmp error3_exit
    jump5:
     
    call atoi
    cmp bx,max_minutes
    ja jump6         
    mov minutes,bl
                 
    xor di,di
        mov dx,4
        mov al,command_line[si]
        cmp al,space_sym
        je jump2 
    duration: 
        mov al,command_line[si]        
        inc si    
        cmp al,dollar
        je duration_end
        cmp dx,0
        je jump2
        cmp al,null_sym
        jb jump2
        cmp al,nine_sym
        ja jump2
        mov buffer_2[di],al
        inc di    
        mov buffer_2[di],dollar
        dec dx
    jmp duration  
    duration_end:
    call atoi
    mov sound_length,bx        
    
   ;alarm clock interrupt (int to bcd)
    xor ax,ax
    mov al,hours
    mov bl,10
    div bl
    mov ch,al
    mov cl,ah
    xor ax,ax
    mov al,ch
    mov bl,16
    mul bl
    add al,cl
    mov hours,al
    xor ax,ax
    mov al,minutes
    mov bl,10
    div bl
    mov ch,al
    mov cl,ah
    xor ax,ax
    mov al,ch
    mov bl,16
    mul bl
    add al,cl
    mov minutes,al
  
    ;setting an alarm clock
    mov ah,07h
    int 1ah
    
    mov ch,hours
    mov cl,minutes
    mov dh,00000000b
    mov ah,06h
    int 1ah 
        
        inc is_resident 
        cli
        mov ah,35h ;saving old interrupt
        mov al,4ah
        int 21h
        mov word ptr cs:old_interrupt,bx
        mov word ptr cs:old_interrupt+2,es
        
        mov ah,25h ;set new interrupt handler
        mov al,4ah  ;4ah
        lea dx,alarm_clock
        int 21h
        sti
        
    jmp resident                             
error1_exit:
    call clear
    cout error1
    jmp exit 
error2_exit:
    call clear
    cout error2
    jmp exit 
error3_exit:
    call clear
    cout error3                    
    jmp exit     
exit:
    mov ax,4c00h
    int 21h
resident:         
    lea dx,installer
    int 27h             
end start