10 PORT(S,A,0):REM PORT A ALS AUSGANG
20 FOR I=0 TO 255
30 PORT(W,A,I):REM SCHREIBE I AN PORT A
40 PAUSE 100
50 NEXT I
60 PAUSE 2000
70 PORT(W,A,0)
