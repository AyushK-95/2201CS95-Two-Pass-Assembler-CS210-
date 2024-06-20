00000000 	     ;/*****************************************************************************
00000000 	     ;TITLE: myAlgo_sum.asm
00000000 	     ;AUTHOR:   Animesh Tripathy
00000000 	     ;Roll No:  2201CS16
00000000 	     ;
00000000 	     ;Declaration of Authorship
00000000 	     ;This test file, is part of the miniproject of CS210 at the
00000000 	     ;department of Computer Science and Engg, IIT Patna .
00000000 	     ;*****************************************************************************/
00000000 	     ; A program to calculate and store the sum of two given numbers
00000000 	     
00000000 00000800 ldc a       ; get address of a
00000001 00000004 ldnl 0      ; store a in reg A
00000002 	     
00000002 00000900 ldc b       ; get address of b; A's shifted to B
00000003 00000004 ldnl 0      ; store b in reg A
00000004 	     
00000004 00000006 add         ;  A = B+A
00000005 	     
00000005 00000a00 ldc result  ; get result address; A's shifted to B
00000006 00000005 stnl 0      ; store B's
00000007 	     
00000007 00000012 HALT        ; HALT will end the program
00000008 	     
00000008 	     
00000008 	     
00000008 00000004 a:  data 4
00000009 00000005 b:  data 5
0000000a 	     
0000000a 00000000 result:    data 0   ; result stored in data
