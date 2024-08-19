; file ps.as
 
.entry LIST 
.extern fn1 
MAIN:            add r3 
MAIN:
jsr fn1 
LOOP: prn #48 
  lea STR, r9 
  inc r6 
  mova *r6, L3 
  sub r11, r4 
  cmp     r3 
  bne  END 
                        add r7, *r6 
  clr K 
  sub      L3, L3 
.entry MAIN 
  jmps LOOP      
END:  stop 
STR:         .string "abcd" 
LIST:            .data 6, -9 
            .data -100 
K:            .data 31
.extern L3 
 
 
