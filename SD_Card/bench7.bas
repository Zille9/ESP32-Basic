10 CLS
100 T=TIMER
120 K=0
125 DIM M(5)
130 K=K+1
140 A=K/2*3+4-5
150 GOSUB 600
155 FOR L=1 TO 5
157 M(L)=A
160 NEXT L
190 IF K<1000 THEN GOTO 130
200 X=(TIMER-T)/1000
210 PRINT X;"sek."
220 PRINT"insgesamt wurden ";INT(K*(15+5));" Zeilen bearbeitet"
230 PRINT"das sind ";INT(K*(15+5)/X);" Zeilen/sek."
300 END
600 RETURN
