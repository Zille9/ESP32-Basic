10 FOR I=0 TO 255
20 PWM(2,I)
30 PAUSE 3
40 NEXT I
50 FOR I=255 TO 0 STEP -1
60 PWM(2,I)
70 PAUSE 3
80 NEXT I
85 PAUSE 500
90 GOTO 10
