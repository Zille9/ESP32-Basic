100 CLS:R=80
110 X=160+65*COS(W):Y=100-65*SIN(W)
120 CIRC X,Y,R,R,0
130 W=W+.2:R=R-2:IF R>0 THEN GOTO 110
