100 CLS:RA=70:RB=30:FOR WA=0 TO 2*PI() STEP 0.05:WB=4*WA
110 XA=160+1.6*(RA*SIN(WA)+RB*SIN(WB))
120 XB=160+1.6*(RA*SIN(WA)-RB*SIN(WB))
130 YA=100+RA*COS(WA)+RB*COS(WB)
140 YB=100+RA*COS(WA)-RB*COS(WB)
150 LINE XA,YA,XB,YB
160 NEXT WA
