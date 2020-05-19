                                       .model small
.stack 100h
.data
leftRangePlatf db 35
rightRangePlatf db 45 
k db 10 
gameOptions1 db "a-left" 
gameOptions2 db "d-right"
platform db "          " 
ball db 'o'
yRange db 23  
px db 40
py db 22 
vx db ?
vy db 1
yTemp db  1
xTemp db ?
naprX db  1
naprY db -1   
bonus db 0   
startMessage db "Input symbol for a start"
scoreMessage db "Score:"       
winMessage db "You win"
lostMessage db "You lost"   
blokString db "          "   
offsetMessage dw 80*23*2+70
score db 0     

.code 
begin: 

    mov ax, @data
    mov ds, ax
    
    mov ax,03h    
    int 10h       
       
    push 0B800h
    pop es  
     
    mov si,offset platform   
    mov di,offsetMessage
    mov bl,16
    mov cx,10
    call writeInVidMem 
     
    sub offsetMessage,5*80*2
    mov si,offset gameOptions1   
    mov di,offsetMessage
    mov bl,14
    mov cx,6 
    call writeInVidMem 
          
    sub offsetMessage,80*2
    mov si,offset gameOptions2   
    mov di,offsetMessage
    mov cx,7 
    call writeInVidMem      
    
    sub offsetMessage,80*2
    mov si,offset startMessage  
    mov di,offsetMessage
    mov cx,24 
    call writeInVidMem 
    
    push offsetMessage 
   
    mov si, offset ball  
    mov di,22*2*80+40*2
    mov al,'o'
    mov bl,14
    mov cx,1       
    call writeInVidMem 
 
    mov si,offset scoreMessage   
    mov di,24*2*80
    mov cx,6  
    mov bl,14  
    call writeInVidMem
  
    mov offsetMessage,3*2*80             
    mov si,offset blokString 
    mov cx,8
    mov bl,32
                    
 cycle:     
    mov di,offsetMessage
    push cx
    mov cx,10      
    call writeInVidMem  
    pop cx   
    add offsetMessage,20
  loop cycle
             
 waitKey:
     mov ah,1h
     int 16h  
     jnz writeStart
   jmp  waitKey
   
writeStart:              

     pop offsetMessage
     mov si,offset startMessage  
     mov di,offsetMessage
     mov cx,24
     mov bl,0 
     call writeInVidMem
                   
     add offsetMessage,80*2  
     lea si,gameOptions1
     mov di,offsetMessage
     mov cx,7 
     call writeInVidMem
    
     add offsetMessage,80*2
     lea si,gameOptions2
     mov di,offsetMessage
     mov cx,6
     call writeInVidMem 
     
checkgenerate: 
    neg naprX     
    
    xor dx,dx
    mov ah,00h
    int 1Ah  
    
    xor ah,ah
    mov al,dl
    mov bx,dx  
    div k 
    
    cmp ah,3                                               
    ja checkgenerate 
    
    cmp ah,0   
    je checkgenerate 
    
    mov vx,ah 
    mov xTemp,ah 

    xor dx,dx
    xor bx,bx  
    
    mov ax,0C00h 
    int 21h             

game:
    cmp dh,24
    jg lost  
    
    call checkWin 
    
    mov     cx,1h
    mov     dx, 189h
    mov     ah, 86h
    int     15h 
       
    mov ah,1h
    int 16h       
       
    cmp al,'d'
    je right  
    cmp al,'a'
    je left  
    
change: 
    mov ah,2 
    mov dh,py
    mov dl,px        
    int 10h      
    
    cmp dl,0
    jge paintOldBall  
    jl  changePos
    
    cmp dl,79
    jle paintOldBall  
    jg   changePos
    
paintOldBall:      
    
    mov px,dl
    mov py,dh 
    
    mov al,' '
    mov bl,0
    mov cx,1        
    call writeChars           
    
   changePos:    
     dec vx  
     add dl,naprX
     cmp vx,0 
     jg changeX
      
     dec vy  
     add dh,naprY
     cmp vy,0 
     jne changeY
             
    je newPos

       changeX: 
       dec vx  
       add dl,naprX 
       cmp vx,0
       jne changeX 
       
       changeY: 
       dec vy   
       add dh,naprY 
       cmp vy,0
       jne changeY 

newPos:  
    mov al,xTemp
    mov vx,al
    mov al,yTemp
    mov vy,al   
   
    cmp dl,leftRangePlatf
    jl checkX1
    cmp dl,rightRangePlatf 
    jg checkX1 
    cmp dh,yRange  
    je reboundPlatform   
           
checkX1:   
    cmp dl,79   
    jge rightWalls   
    
checkX2:  
    cmp dl,0 
    jle leftWalls        
   
checkY1:   
    cmp dh,0   
    jle upWalls   
    
paint:         

    mov px,dl
    mov py,dh   
    mov ah,2   
    int 10h   
    
    mov ah,8
    int 10h 
    
    cmp ah,16 
    jb compare

blok: 
    cmp dh,yRange
    je compare
    
    add score,1     
    mov ah,2
    mov dh,py
    mov dl,px
    int 10h
          
    mov al,' '
    mov cx,1
    mov bl,0
    call writeChars   
     
    add dh,2 
    mov ah,2
    int 10h
    
    mov al,' '
    mov cx,1
    mov bl,0
    call writeChars
    
    dec dh
    neg naprY 
    add dl,naprX

    cmp dl,0
    jg nextCheckBlok 
    jmp notOnlyBlok

nextCheckBlok:
    cmp dl,79
    jl onlyBlok  

notOnlyBlok: 
    add dh,naprY
    add dh,naprY   
    mov px,dl
    mov py,dh     
    
    call paintScore
    jmp compare 

onlyBlok:    

    add dh,naprY 
    mov px,dl      
    mov py,dh    
    call paintScore
 
compare:    
 
    cmp dl,leftRangePlatf            
    jl checkWalls
    cmp dl,rightRangePlatf 
    jge  checkWalls 
    cmp dh,yRange  
    je reboundPlatform
  
checkWalls:                     

    cmp dh,0
    jle upWalls         
    cmp dl,79
    jg rightWalls 
    cmp dl,0
    jle leftwalls

paint_:  
 
    mov ah,2 
    mov dh,py
    mov dl,px        
    int 10h      
    mov al,'o'
    mov bl,14    
    call writeChars
                    
jmp  game
         
right:        
    mov ax,0C00h 
    int 21h
    call rightProc
jmp  change 

left:        
    mov ax,0C00h 
    int 21h
    call leftProc
jmp  change 

reboundPlatform:        
    mov ax,0C00h 
    int 21h
    call  reboundPlatformProc
jmp  game  

rightWalls:   
    mov ax,0C00h 
    int 21h
    call leftWallsProc  
jmp game 

upWalls:    
    mov ax,0C00h 
    int 21h
    call upWallsProc
jmp game         
      
leftWalls:  
    mov ax,0C00h 
    int 21h
    call leftWallsProc
   jmp game         
   
rightProc proc        
  pusha    
   cmp rightRangePlatf,79
   jg exitFromFun     
   
   mov ah,2
   mov dh,yRange
   mov dl,leftRangePlatf
   int 10h 
 
   mov al,' '
   mov bl,0
   call writeChars
  
   add dl,1
   mov leftRangePlatf,dl 
      
   mov ah,2
   mov dh,yRange
   mov dl,leftRangePlatf
   int 10h 
         
   mov al,' '
   mov bl,0
   call writeChars
    
   add dl,1
   mov leftRangePlatf,dl 
   mov ah,2

   mov dl,rightRangePlatf
   int 10h 

   mov al,' '
   mov bl,16
   call writeChars  
   
   add rightRangePlatf,1    
   cmp rightRangePlatf,79
   jge addLength       
   mov ah,2
   mov dl,rightRangePlatf
   int 10h 
  
   mov al,' '
   mov bl,16
   call writeChars  
   
   add rightRangePlatf,1
   jmp exitFromFun
  addlength:  
   dec  leftRangePlatf
   mov ah,2
   mov dh,yRange
   mov dl,leftRangePlatf
   int 10h      
   mov al,' '
   mov bl,16
   call writeChars 
  exitFromFun:                      
    popa
    ret
rightProc endp                             
      
leftProc proc  
    pusha 
     
    cmp leftRangePlatf,1
    jl exitFromFun1   
    
    mov ah,2
    mov dh,yRange
    mov dl,rightRangePlatf  
    dec dl
    int 10h  
    
    mov ah,9
    mov al,' '  
    mov bl,0
    mov cx,1
    int 10h      
    
    mov rightRangePlatf,dl    
         
    mov ah,2
    mov dh,yRange
    mov dl,rightRangePlatf  
    dec dl
    int 10h    
     
    mov ah,9
    mov al,' '  
    mov bl,0
    mov cx,1
    int 10h 
    mov rightRangePlatf,dl 
            
    mov ah,2  
    
    mov dl,leftRangePlatf  
    dec dl
    int 10h 
    mov ah,9
    mov al,' '
    mov bl,16
    mov cx,1
    int 10h   
    
    mov leftRangePlatf,dl   
    mov ah,2                                   
                                          
    mov dl,leftRangePlatf  
    dec dl
    int 10h 
    mov ah,9
    mov al,' '
    mov bl,16
    mov cx,1
    int 10h 
    mov leftRangePlatf,dl
      
    exitFromFun1:                 
    popa
    ret
leftProc endp     
  
reboundPlatformProc proc
    pusha   
    mov ah,2
    mov dh,py 
    dec dh 
    
    neg naprY
    mov dl,px
    add dl,naprX   
    
    cmp dl,1
    jge nextCheckAfterPlatf
    mov dl,0 
    neg naprX  
    add dl,naprX
    jmp write  
    
    nextCheckAfterPlatf:
    cmp dl,78
    jle write
    mov dl,79 
    neg naprX 
    add dl,naprX
    
    write:  
    add dh,naprY 
    mov ah,2
    mov px,dl
    mov py,dh  
    int 10h
    
    xor bh,bh
    mov bl,14
    mov al,'o' 
    call writeChars
    
    popa
    ret  
reboundPlatformProc endp 
  
upWallsProc proc
    pusha          
    
    mov ah,2
    mov dl,px   
    mov dh,py
    int 10h 
    mov al,' '
    mov bl,0
    call writeChars
    neg naprY                
    
    popa
    ret    
upWallsProc endp

checkWin proc
pusha  

    mov ah,2
    mov dh,3
    mov dl,0
    int 10h

check:
    mov ah,8
    int 10h
    cmp ah,16
    jae exitFromCheckWin
    cmp dl,79
    jge exitIfWin 
    add dl,1
    mov ah,2
    int 10h
    jmp check

lost:

   mov si,offset lostMessage   
   mov di,12*80*2+80
   mov bl,14
   mov cx,8
   call writeInVidMem
   mov ax,4c00h
   int 21h

exitIfWin:
    
   mov si,offset winMessage   
   mov di,12*80*2+80
   mov bl,16
   mov cx,7
   call writeInVidMem 
   mov ax,4c00h
   int 21h

exitFromCheckWin:   
    popa  
    ret
  
checkWin endp   

paintScore proc    
    pusha  
 
    generateBonusScore:  
    mov ah,00h
    int 1Ah    
    
    xor ah,ah
    mov al,dl
    div k
    cmp ah,0
    je generateBonusScore    
    
    mov al,ah
    xor ah,ah
    mov k,al
    mov al,score
    div k  
    mov k,10
    cmp ah,0    
    jne paintNotBonus    
    
    mov bh,1     
    add bonus,bh 
    xor bx,bx
    xor al,al 
    mov dh,24
    mov dl,39
    mov ah,2  
    int 10h   

    mov cx,1
    mov bl,14
    mov al,'X'   
    call writeChars

    add dl,1
    mov ah,2  
    int 10h  

    mov cx,1
    mov bl,14
    mov al,'2'   
    call writeChars

    jmp paintScoreMessage  
      
paintNotBonus:
    xor bx,bx
    xor al,al 
    mov dh,24
    mov dl,39
    mov ah,2  
    int 10h    
    
    mov cx,1
    mov bl,14
    mov al,'X'   
    call writeChars
    add dl,1 
    
    mov ah,2  
    int 10h 
    mov bl,14
    mov al,'1' 
    call writeChars     
    
paintScoreMessage: 
    mov bh,bonus
    add score,bh
    mov ah,2
    mov dh,24
    mov dl,7
    int 10h     
    
    xor cx,cx  
    xor bx,bx
    xor ah,ah
    mov al,score 
    mov bonus,0 
    
perevod:  

    div k  
    add cx,1
    mov bl,al
    mov al,ah
    xor ah,ah 
    add al,'0'
    push ax 
    cmp bl,0
    je setPosScore 
    mov al,bl
  jmp perevod      
    
setPosScore:        

    xor ah,ah
    mov ah,2  
    mov dl,7  
    mov dh,24
    int 10h 
    
outputScore:            

    pop ax 
    mov bl,14
    mov dh,cl
     
call writeChars
  
    mov cl,dh 
    mov ah,2 
    mov dh,24
    add dl,1
    int 10h
    loop outputScore
    popa    
    ret         
    
paintScore endp    

leftWallsProc proc
pusha         
  
    neg naprX  
    mov ah,2
    int 10h

    mov px,dl 
    mov py,dh  
    
    mov ah,8
    int 10h
    add dl,naprX    
    add dl,naprX
    mov px,dl 
    mov py,dh 
     
    cmp ah,16
    jl game
    cmp dh,3
    jge game
    neg naprY  
 
    popa
    ret    
leftWallsProc endp       

writeInVidMem proc
    pusha  
    cycle_:
    mov al,[si]
    mov es:[di],al
    inc di 
    mov es:[di],bl
    inc si
    inc di 
    loop cycle_
    popa
    ret    
writeInVidMem endp   

writeChars proc
     pusha
     mov ah,9 
     mov cx,1
     int 10h    
     popa
     ret
writeChars endp
        
writeStrings proc
pusha    
mov ah,13h
    mov al,0
    int 10h   
popa
ret    
writeStrings endp

end begin