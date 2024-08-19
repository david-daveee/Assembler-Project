MAIN: add r3, LIST 
LOOP: prn #48 
macr m_macr       
      cmps r3, #-6 
      bne  END 
endmacr
      lea STR, r6 
      inc r6 
      mov *r6,K 
      sub r1, r9 
      m_macr
      dec K 
      jmp LOOP, r1      
END:  stops 
STR:  .string "abcd" 
LIST: .data 6, -9 
      .data -100 
K:    .data 31
