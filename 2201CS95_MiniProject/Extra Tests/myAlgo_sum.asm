;/*****************************************************************************
;TITLE: myAlgo_sum.asm																																
;AUTHOR:   Animesh Tripathy
;Roll No:  2201CS16
;
;Declaration of Authorship
;This test file, is part of the miniproject of CS210 at the 
;department of Computer Science and Engg, IIT Patna . 
;*****************************************************************************/
; A program to calculate and store the sum of two given numbers

ldc a       ; get address of a
ldnl 0      ; store a in reg A

ldc b       ; get address of b; A's shifted to B
ldnl 0      ; store b in reg A

add         ;  A = B+A

ldc result  ; get result address; A's shifted to B
stnl 0      ; store B's

HALT        ; HALT will end the program



a:  data 4
b:  data 5

result:    data 0   ; result stored in data