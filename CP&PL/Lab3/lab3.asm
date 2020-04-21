#make_exe#  ;compile as .exe file

.186  ;for POPA/PUSHA
.model small
   ;.org 100h - no need in .exe program

.stack 100h
                                   
                                   



.data
newl     EQU 0Ah, 0Dh
MAX_EL   EQU 30

msgElArray  db "Write the elements of array:", newl, '$'
msgRangeL   db newl, "Write the lower end of range: ", newl, '$'
msgRangeH   db newl, "Write higher end of range: ", newl, '$'
msgArrLen   db newl, "Recognized numbers: ", '$'
msgResult   db newl, "Number of matches: ", '$'
msgError    db newl, "Error! Code: ", '$'
msgNewl     db newl, '$'

strInput    db 255, ?, 255 DUP(?);numbers in string form
numAmount   dw ?                 ;size of number array
numArray    dw MAX_EL DUP(?)     ;prepared array of signed numbers
strRangeL   db 7, ?, 7 DUP(?)    ;low bound in string form
rangeLow    dw ?                 ;same as a signed number
strRangeH   db 7, 8 DUP(?)    ;high bound in string form
rangeHigh   dw ?                 ;same as a signed number
strResult   db 7 DUP(?)          
valResult   db ?


.code
start:
mov ax, @data  ;Writes correct data into ds and es
mov ds, ax     ;Important for multi-segment programs
mov es, ax



 
         
;ask for number string         
mov dx, offset msgElArray
call cout
;user inputs string
mov dx, offset strInput
call cin

;ask for low bound        
mov dx, offset msgRangeL
call cout
;user inputs string
mov dx, offset strRangeL
call cin

;ask for high bound
mov dx, offset msgRangeH
call cout
;user inputs string
mov dx, offset strRangeH
call cin

;Input done, now convert all strings to numbers

;convert low bound
mov di, offset strRangeL + 2
call strToWord
jnc cErr1
   mov al, '1'
   call thrErr 
cErr1:
mov rangeLow, ax    

;convert high bound
mov di, offset strRangeH + 2
call strToWord
jnc cErr2
   mov al, '2'
   call thrErr
cErr2:
mov rangeHigh, ax


;Now, check if ranges are correct. If not, swap them
cmp rangeLow, ax
jle noRangeSwap
   xchg rangeLow, ax
   mov rangeHigh, ax
noRangeSwap:

   
mov cx, MAX_EL
mov di, offset strInput + 2
mov si, offset numArray
convLoop:
   call strToWord
   jc convLoopEnd
   mov [si], ax
   add si, 2
loop convLoop 
convLoopEnd:    
   ;can check if read all elements. No need for now.
   ;Put array size in its variable
mov numAmount, MAX_EL
sub numAmount, cx



;Output amount of converted numbers
mov dx, offset msgArrLen
call cout
mov ax, numAmount
mov si, offset strResult   ;reuse array from result
call wordToStr
mov dx, si
call cout
mov dx, offset msgNewl
call cout



;Seems we got the numbers. Now, simple compare routine
xor ax, ax  ;use AX to store matches 
mov cx, numAmount
test cx, cx
jz endCountLoop   ;on empty input loop underflows to FFFFh - obv. bad
mov si, offset numArray
countLoop:
   mov dx, [si]
   cmp dx, rangeLow
   jl noCount
   cmp dx, rangeHigh
   jg noCount
   inc ax
   noCount:
   add si, 2
loop countLoop   
endCountLoop:




;Output result in console
mov si, offset strResult
call wordToStr
mov dx, offset msgResult
call cout
mov dx, si
call cout
mov dx, offset msgNewl
call cout

mov ah, 1   ;pause, wait for a key press
int 21h

mov ah, 4Ch
int 21h




         ;-------FUNCTIONS-------

;mov bx, -5


;Output to screen. Needs ptr to '$'-terminated string in DX
proc cout
   mov ah, 09h
   int 21h
   ret   
endp cout

;Inputs string from console. Needs ptr to correct structure in DX
proc cin 
   mov ah, 0Ah
   int 21h
   ret
endp cin   

;put letter into AL before calling!(will output it after message)
proc thrErr
   mov dx, offset msgError
   call cout
   cmp al, '!'  ;output '?' if invis symbol
   jl dflSymb
   cmp al, '~'
   jg dflSymb
   mov dl, al
   mov ah, 02h
   int 21h
   int 20h
      
   dflSymb:   
   mov dl, '?'
   mov ah, 02h
   int 21h
   int 20h
   
   ret
endp thrErr


;Transforms base-10 string (+num/num/-num) 
;into SIGNED word(-32768..+32767, 6 symbols max)
;Input - pointer to ACTUAL string in DI(will skip ' 's)
;Outputs - number into AX, on error - sets CF to 1 
proc strToWord
   push bx
   push cx
   push dx
   
   mov bx, 7      ;Has clever use later (last digit of +-MAX word)
   ;skipping initial ' 's
   mov cx, 0FFFFh  ;(up to this amount)
   mov al, ' '
   repe SCASB
   dec di   ;goes 1 past the wrong symbol, fix that
   xor ax, ax
   
      
   ;read 1st symbol, note if '-'
   ;read digits in loop, on anything wrong throw error
   cmp [di], '+'
   je positExpl
   cmp [di], '-'
   jne posit
      mov bx, 8   ;not a random number, has use later
      positExpl:
      inc di   
   posit: 
   cmp [di], '0'    ;needs to have at least one digit to be a number
   jl throwError
   cmp [di], '9'
   jg throwError
   
   mov cx, 0   ;in case number has a bunch of leading zeros(will underflow to 0FFFFh)   
   stwLoop:
      ;cmp [di], ' '    ;Number must end with ' '   (can be configured here)
      ;je addingSign
      cmp [di], '0'     ;Ran out of digits. Output result
      jl addingSign
      cmp [di], '9'    
      jg addingSign
      
      ;signed over/underflow check
      cmp ax, 3276d  ;check before adding last digit
      jl notOvf      ;all ok, signed number will be in bounds
      jg throwError    ;definitely overflow 
         mov bh, [di]
         sub bh, '0'
         cmp bl, bh     ;Clever use of sign info to check last digit                 ;---BUG HERE--- ?                           
         jl throwError  ;slightly beyond +-MAX signed word, can't fit that              
      notOvf:
      ;all checks passed, can add another digit
      push bx
      mov bx, 10 ;shift AX in decimal. Can't plug 10 directly
      mul bx
      xor bx, bx      
      mov bl, [di]
      sub bl, '0'
      add ax, bx
      pop bx
      
      inc di  
   loop stwLoop 
   loopEnd:
   
   ;check if there's no digits left
   cmp [di], '0'
   jl addingSign
   cmp [di], '9'
   jg addingSign
   jmp throwError 
    
   addingSign:   ;makes the number signed
   cmp bx, 8
   jne exitProc
   neg ax
   jmp exitProc         
         
   throwError:        ;need to properly unset flag!
   mov ax, 0
   stc
   pop dx
   pop cx
   pop bx
   ret
   exitProc:
   clc
   pop dx
   pop cx
   pop bx
   ret
endp strToWord   



;converts 2-byte SIGNED number into 7-char string (with '$' at the end)
;Input - signed int in AX, pointer to string in SI
;Output - in string at SI
;(registers don't change)
proc wordToStr
   ;Algorithm:
   ;if negative, store(in 1st char) and NEG, otherwise skip
   ;then, get digits from left to right.
   pusha
   
   cmp ax, 0
   jg wtsCont
   jl wtsNeg   ;if zero, easy case:
      mov [si], '0'
      inc si
      mov [si], '$'
      popa
      ret
   wtsNeg:
      mov [si], '-'
      inc si
      neg ax
   wtsCont:
   mov bx, 10000
   mov cx, 0FF05h  ;top half used as flag, will be 00h after 1st non-zero digit
   wtsLoop:
      xor dx, dx
      div bx   
      
      ;don't print leading zeros
      test ax, ax
      jnz wtsDoPrint
      test ch, ch
      jnz wtsNoPrint
      wtsDoPrint:
      mov ch, 00h    ;CH is used as 'leading zero' flag
      add ax, '0' 
      mov [si], ax
      inc si 
      wtsNoPrint:
      
      mov ax, dx

      push ax    
      mov ax, bx
      mov bx, 10
      xor dx, dx
      div bx
      mov bx, ax
      pop ax 
   loop wtsLoop
   mov [si], '$' 
   
   popa
   ret
endp wordToStr





end start