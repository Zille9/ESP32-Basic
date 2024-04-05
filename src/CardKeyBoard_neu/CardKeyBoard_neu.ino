//
// CardKeyBoard.ino
// PS/2 Version - Author:Herr Tamakichi
// 2019/03/08
// Erforderliche Bibliothek
// ps2dev https://playground.arduino.cc/uploads/ComponentLib/ps2dev.zip
// Adafruit_NeoPixel
//
// 2023/12/30 stark angepasste Version
// für die korrekte Darstellung muss am Host-Computer die japanische Tastatur angegeben werden
//
// **********************************************************************************************************************************
// * HINWEIS ZUM BRENNEN: MiniCore 328P - no Bootloader - 8MHz intern - STK500 as ISP (MiniCore) einstellen                         *
// * seltsamerweise funktioniert der Controller erst nach dem Versuch den Bootloader zu brennen - wird mit Fehlermeldung quittiert  *
// * danach das Programm flashen - dann sollte alles funktionieren                                                                  *
// *                                                                                                                                *
// * Hinweis zum Keyboard-PCB V1.0 - MISO und MOSI sind vertauscht, das muss berücksichtigt werden, damit der Brenner funktioniert  *
// **********************************************************************************************************************************
// * noch startet der Tastatur treiber nicht immer korrekt -> behoben (siehe loop kayboard.available() - variable startup - wird gesetzt
// * funktioniert aber nur beim Einschalten - Resetverbindung zur Hostanwendung würde beim Reset die Variable zurücksetzen
// * Auto-Repeat hinzugefügt
// * Funktion soweit zufriedenstellend 
//
#include <Adafruit_NeoPixel.h>
#include <ps2dev.h>
#define KB_CLK      A4  // PS/2 CLK  
#define KB_DATA     A5  // PS/2 DATA 

#define PIN           13
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
PS2dev keyboard(KB_CLK, KB_DATA); // PS/2

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)     (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

//       d0   d1     d2  d3 d4 d5 d6 d7 d8 d9 d10 d11
//A3：   esc   1      2  3  4  5  6  7  8  9  0   del
//A2:    tab   q      w  e  r  t  y  u  i  o  p
//A1:   left   up     a  s  d  f  g  h  j  k  l   enter
//A0:   down   right  z  x  c  v  b  n  m  ,  .   space
//sym: d15
//shift: d12
//fn: d14

int16_t old_no;
int startup = 0;
unsigned char KeyMap[48][7] =
{ //nor, shift,long_shift, sym,long_sym,fn,long_fn,
  {  27,  27,  27,  27,  27, 128, 128},//esc
  { '1', '1', '1', '!', '!', 129, 129},//1
  { '2', '2', '2', '"', '"', 130, 130},//2
  { '3', '3', '3', '\'', '\'', 131, 131},//3
  { '4', '4', '4', '$', '$', 132, 132},//4
  { '5', '5', '5', '%', '%', 133, 133},//5
  { '6', '6', '6', '&', '&', 134, 134},//6
  { '7', '7', '7', '/', '/', 135, 135},//7
  { '8', '8', '8', '(', '(', 136, 136},//8
  { '9', '9', '9', ')', ')', 137, 137},//9
  { '0', '0', '0', '=', '=', 138, 138},//0
  {  8 , 127 , 127,  8 ,  8 , 139, 139}, //del
  {  9 ,  9 ,  9 ,  9 ,  9 , 140, 140},//tab
  { 'q', 'Q', 'Q', '{', '{', 141, 141},//q
  { 'w', 'W', 'W', '}', '}', 142, 142},//w
  { 'e', 'E', 'E', '+', '+', 143, 143},//e
  { 'r', 'R', 'R', '#', '#', 144, 144},//r
  { 't', 'T', 'T', '[', '[', 145, 145},//t
  { 'y', 'Y', 'Y', ']', ']', 146, 146}, //y
  { 'u', 'U', 'U', '|', '|', 147, 147},//u
  { 'i', 'I', 'I', '~', '~', 148, 148},//i
  { 'o', 'O', 'O', '^', '^', 149, 149}, //o
  { 'p', 'P', 'P', '*', '*', 150, 150},//p
  {  0 ,  0 ,  0 ,  0 ,  0 , 0, 0},    //  no key
  {  180,  180,  180,  180,  180, 152, 152},//LEFT
  {  181,  181,  181,  181,  181, 153, 153},//UP
  { 'a', 'A', 'A', '<', '<', 154, 154},//a
  { 's', 'S', 'S', '>', '>', 155, 155},//s
  { 'd', 'D', 'D', '@', '@', 156, 156},//d
  { 'f', 'F', 'F', 0, 0, 157, 157},//f
  { 'g', 'G', 'G', '-', '-', 158, 158},//g
  { 'h', 'H', 'H', '_', '_', 159, 159},//h
  { 'j', 'J', 'J', 0, 0, 160, 160},//j
  { 'k', 'K', 'K', '?', '?', 161, 161},//k
  { 'l', 'L', 'L',   0,   0, 162, 162},//l
  { 13 , 13 ,  13,  13,  13, 163, 163},//enter
  {  182, 182 ,  182,  182,  182, 164, 164},//DOWN
  {  183, 183 ,  183,  183,  183, 165, 165},//RIGHT
  { 'z', 'Z', 'Z',  0 ,  0 , 166, 166},//z
  { 'x', 'X', 'X',  0 ,  0 , 167, 167},//x
  { 'c', 'C', 'C',  0 ,  0 , 168, 168},//c
  { 'v', 'V', 'V',  0 ,  0 , 169, 169},//v
  { 'b', 'B', 'B',  0 ,  0 , 170, 170},//b
  { 'n', 'N', 'N',  0 ,  0 , 171, 171},//n
  { 'm', 'M', 'M',  0 ,  0 , 172, 172},//m
  { ',', ',', ',', ';', ';', 173, 173},//,
  { '.', '.', '.', ':', ':', 174, 174},//.
  { ' ' , ' ', ' ', ' ', ' ', 175, 175}//space
};

// ASCII-Code-zu-Scan-Code-Konvertierungstabelle
unsigned char ScanMap[121][3] = {
  // Zeichencode, 2 Bytes | Verschiebung, Scancode
  { 8, 0, 0x66 } , // Backspace
  { 9, 0, 0x0D } , // TAB
  { 13, 0, 0x5A } , // Enter
  { 27, 0, 0x76 } , // Esc
  { 32, 0, 0x29 } , // Space
  { 33, 1, 0x16 } , // !
  { 34, 1, 0x1E } , // "
  { 35, 1, 0x26 } , // #
  { 36, 1, 0x25 } , // $
  { 37, 1, 0x2E } , // %
  { 38, 1, 0x36 } , // &
  { 39, 1, 0x3D } , // '
  { 40, 1, 0x3E } , // (
  { 41, 1, 0x46 } , // )
  { 42, 1, 0x52 } , // *
  { 43, 1, 0x4C } , // +
  { 44, 0, 0x41 } , // ,
  { 45, 0, 0x4E } , // -
  { 46, 0, 0x49 } , // .
  { 47, 0, 0x4A } , // /
  { 48, 0, 0x45 } , // 0
  { 49, 0, 0x16 } , // 1
  { 50, 0, 0x1E } , // 2
  { 51, 0, 0x26 } , // 3
  { 52, 0, 0x25 } , // 4
  { 53, 0, 0x2E } , // 5
  { 54, 0, 0x36 } , // 6
  { 55, 0, 0x3D } , // 7
  { 56, 0, 0x3E } , // 8
  { 57, 0, 0x46 } , // 9
  { 58, 0, 0x52 } , // :
  { 59, 0, 0x4C } , // ;
  { 60, 1, 0x41 } , // <
  { 61, 1, 0x4E } , // =
  { 62, 1, 0x49 } , // >
  { 63, 1, 0x4A } , // ?
  { 64, 0, 0x54 } , // @
  { 65, 1, 0x1C } , // A
  { 66, 1, 0x32 } , // B
  { 67, 1, 0x21 } , // C
  { 68, 1, 0x23 } , // D
  { 69, 1, 0x24 } , // E
  { 70, 1, 0x2B } , // F
  { 71, 1, 0x34 } , // G
  { 72, 1, 0x33 } , // H
  { 73, 1, 0x43 } , // I
  { 74, 1, 0x3B } , // J
  { 75, 1, 0x42 } , // K
  { 76, 1, 0x4B } , // L
  { 77, 1, 0x3A } , // M
  { 78, 1, 0x31 } , // N
  { 79, 1, 0x44 } , // O
  { 80, 1, 0x4D } , // P
  { 81, 1, 0x15 } , // Q
  { 82, 1, 0x2D } , // R
  { 83, 1, 0x1B } , // S
  { 84, 1, 0x2C } , // T
  { 85, 1, 0x3C } , // U
  { 86, 1, 0x2A } , // V
  { 87, 1, 0x1D } , // W
  { 88, 1, 0x22 } , // X
  { 89, 1, 0x1A } , // Y
  { 90, 1, 0x35 } , // Z
  { 91, 0, 0x5B } , // [
  { 92, 0, 0x6A } , // \
  { 92, 0, 0x51 } , // ＼
  { 93, 0, 0x5D } , // ]
  { 94, 0, 0x55 } , // ^
  { 95, 1, 0x51 } , // _
  { 96, 1, 0x54 } , // `
  { 97, 0, 0x1C } , // a
  { 98, 0, 0x32 } , // b
  { 99, 0, 0x21 } , // c
  { 100, 0, 0x23 } , // d
  { 101, 0, 0x24 } , // e
  { 102, 0, 0x2B } , // f
  { 103, 0, 0x34 } , // g
  { 104, 0, 0x33 } , // h
  { 105, 0, 0x43 } , // i
  { 106, 0, 0x3B } , // j
  { 107, 0, 0x42 } , // k
  { 108, 0, 0x4B } , // l
  { 109, 0, 0x3A } , // m
  { 110, 0, 0x31 } , // n
  { 111, 0, 0x44 } , // o
  { 112, 0, 0x4D } , // p
  { 113, 0, 0x15 } , // q
  { 114, 0, 0x2D } , // r
  { 115, 0, 0x1B } , // s
  { 116, 0, 0x2C } , // t
  { 117, 0, 0x3C } , // u
  { 118, 0, 0x2A } , // v
  { 119, 0, 0x1D } , // w
  { 120, 0, 0x22 } , // x
  { 121, 0, 0x1A } , // y
  { 122, 0, 0x35 } , // z
  { 123, 1, 0x5B } , // {
  { 124, 1, 0x6A } , // |
  { 125, 1, 0x5D } , // }
  { 126, 1, 0x55 } , // ~
  { 127, 2, 0x71 } , // Delete
  { 129, 0, 0x05 } , // F1
  { 130, 0, 0x06 } , // F2
  { 131, 0, 0x04 } , // F3
  { 132, 0, 0x0C } , // F4
  { 133, 0, 0x03 } , // F5
  { 134, 0, 0x0B } , // F6
  { 135, 0, 0x83 } , // F7
  { 136, 0, 0x0A } , // F8
  { 137, 0, 0x01 } , // F9
  { 138, 0, 0x09 } , // F10
  { 139, 2, 0x70 } , // Insert
  { 152, 2, 0x6C } , // Home
  { 153, 2, 0x7D } , // PgUp
  { 161, 0, 0x13 } , // Kana
  { 164, 2, 0x7A } , // PgDn
  { 165, 2, 0x69 } , // End
  { 180, 2, 0x6B } , // ←
  { 181, 2, 0x75 } , // ↑
  { 182, 2, 0x72 } , // ↓
  { 183, 2, 0x74 } , // →
};

uint8_t enabled = 0;              // PS/2 Host zum Senden bereit

// Durchsucht die Scan-Code-Konvertierungstabelle
int16_t findScanMap(uint8_t code) {
  int16_t no = -1;
  for (int16_t i = 0; i < 121; i++) {
    if (ScanMap[i][0] == code) {
      no = i;
      break;
    }
  }
  return no;
}

// PS/2 Sende eine Bestätigung an den Host
void ack() {
  while (keyboard.write(0xFA));
}

// PS/2 Vom Host gesendete Befehle verarbeiten
int keyboardcommand(int command) {
  unsigned char val;
  uint32_t tm;
  switch (command) {
    case 0xFF:
      ack();// Reset: Befehl zum Zurücksetzen der Tastatur. Gibt ACK zurück, wenn es korrekt empfangen wurde.
      while(keyboard.write(0xAA)!=0);
      break;
    case 0xFE: // Anfrage erneut senden
      ack();
      break;
    case 0xF6: // Kehre zum Startzustand zurück
      //enter stream mode
      ack();
      break;
    case 0xF5: //Kehre zum Startzustand zurück und stoppe den Keyscan
      //FM
      enabled = 0;
      ack();
      break;
    case 0xF4: //Keyscan starten
      //FM
      enabled = 1;
      ack();
      break;
    case 0xF3: //set typematic rate/delay :
      ack();
      if(!keyboard.read(&val)) ack(); //do nothing with the rate
      break;
    case 0xF2: //get device id :
      ack();
      keyboard.write(0xAB);
      keyboard.write(0x83);
      break;
    case 0xF0: //set scan code set
      ack();
      if(!keyboard.read(&val)) ack(); //do nothing with the rate
      break;
    case 0xEE: //echo :Wenn eine Tastatur angeschlossen ist, reagiert die Tastatur auf den Computer.（ECHO Response）Gib es zurück.
      //ack();
      keyboard.write(0xEE);
      break;
    case 0xED: //set/reset LEDs :Anfrage zum Ein-/Ausschalten der Tastatur-LED. Geben Sie die LED mit dem folgenden Optionsbyte an.
      ack();
      if(!keyboard.read(&val)) ack();; //do nothing with the rate
      ack();
      return 1;
      break;
  }
return 1;
}

// Scancode senden
// break
// no : Datensatznummer der Scancode-Tabelle
void sendScanCode(int16_t no) {

  // Code senden
  if (ScanMap[no][1] & 1) {
    // Wenn die Umschalttaste zusammen verwendet wird, senden Sie zuerst den Scancode der rechten Umschalttaste.
    keyboard.write(0x59);
  }


  if (ScanMap[no][1] & 2) {
    // 2-Byte-Scancode
    keyboard.write(0xe0);
  }

  //keyboard.write(ScanMap[no][2]);
  keyboard.keyboard_mkbrk(ScanMap[no][2]);
  delay(10);

  // Break Code senden
  if (ScanMap[no][1] & 2) {
    // 2-Byte-Scancode
    keyboard.write(0xe0);
    
  }
  keyboard.write(0xf0);
  keyboard.write(ScanMap[no][2]);
  delay(10);

  if (ScanMap[no][1] & 1) {
    // Wenn die Umschalttaste zusammen verwendet wird, senden Sie zuletzt den Scancode der rechten Umschalttaste.
    keyboard.write(0xf0);
    keyboard.write(0x59);
  }
  delay(10);
}

#define shiftPressed (PINB & 0x10 ) != 0x10
#define symPressed (PINB & 0x80 ) != 0x80
#define fnPressed (PINB & 0x40 ) != 0x40

int _shift = 0, _fn = 0, _sym = 0, idle = 0;
unsigned char KEY = 0, hadPressed = 0;
int Mode = 0; //0->normal.1->shift 2->long_shift, 3->sym, 4->long_sym 5->fn,6->long_fn

void flashOn() {
  pixels.setPixelColor(0, pixels.Color(3, 3, 3)); pixels.show();
}

void flashOff() {
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); pixels.show();
}


void setup() {
  //Serial.begin(115200);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  DDRB = 0x00;
  PORTB = 0xff;
  DDRD = 0x00;
  PORTD = 0xff;

  pixels.begin();
  keyboard.keyboard_init();
  
}

unsigned char GetInput() {
  unsigned char k = 0;
  digitalWrite(A3, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A0, HIGH);
  //delay(2);
  if (PIND != 0xff) {
    //flashOn();
    switch (PIND)   {
      case 254: k = 1; break; //ESC
      case 253: k = 2; break; // 1
      case 251: k = 3; break; //2
      case 247: k = 4; break; //3
      case 239: k = 5; break; //4
      case 223: k = 6; break; //5
      case 191: k = 7; break; //6
      case 127: k =  8; break; //7
    }
    /*
      flashOff();
      if (k) {
      hadPressed = 1;
      return k;
      }
    */
  }
  else if (PIND != 223) {
    //flashOn();
    //delay(200);
    switch (PINB)   {
      case 222: k = 9; break; //8
      case 221: k = 10; break; //9
      case 219: k = 11; break; //0
      case 215: k = 12; break; //Del
    }


  }
  //delay(200);
  //flashOff();
  if (k) {
    hadPressed = 1;
    return k;
  }

  digitalWrite(A3, HIGH);
  digitalWrite(A2,  LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A0, HIGH);
  //delay(2);
  if (PIND != 0xff) {
    //flashOn();
    //delay(200);
    switch (PIND)   {
      case 254: k = 13; break; //Tab
      case 253: k = 14; break; //Q
      case 251: k = 15; break; //W
      case 247: k = 16; break; //E
      case 239: k = 17; break; //R
      case 223: k = 18; break; //T
      case 191: k = 19; break; //Y
      case 127: k = 20; break; //U
    }
    //flashOff();
    //if (k) {
    //  hadPressed = 1;
    //  return k;
    // }
  }
  else if (PIND != 223) {
    //flashOn();
    switch (PINB)   {
      case 222: k = 21; break; //I
      case 221: k = 22; break; //O
      case 219: k = 23; break; //P
      case 215: k = 24; break; //no key
    }

  }
  //delay(200);
  //flashOff();
  if (k) {
    hadPressed = 1;
    return k;
  }




  digitalWrite(A3, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A0, HIGH);
  //delay(2);
  if (PIND != 0xff) {
    //flashOn();
    //delay(200);
    switch (PIND)   {
      case 254: k = 25; break; //left
      case 253: k = 26; break; //up
      case 251: k = 27; break; //A
      case 247: k = 28; break; //S
      case 239: k = 29; break; //D
      case 223: k = 30; break; //F
      case 191: k = 31; break; //G
      case 127: k =  32; break; //H
    }
    //flashOff();
    //if (k) {
    //  hadPressed = 1;
    //  return k;
    //}
  }

  else if (PIND != 223) {
    //flashOn();
    //delay(200);
    switch (PINB)   {
      case 222: k = 33; break; //J
      case 221: k = 34; break; //K
      case 219: k = 35; break; //L
      case 215: k = 36; break; //Enter
    }
  }
  //flashOff();
  if (k) {
    hadPressed = 1;
    return k;

  }

  digitalWrite(A3, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A0, LOW);
  //delay(2);

  if (PIND != 0xff) {
    //flashOn();
    //delay(200);

    switch (PIND)   {
      case 254: k = 37; break; //down
      case 253: k = 38; break; //right
      case 251: k = 39; break; //Z
      case 247: k = 40; break; //X
      case 239: k = 41; break; //C
      case 223: k = 42; break; //V
      case 191: k = 43; break; //B
      case 127: k = 44; break; //N
    }
    //flashOff();

    //if (k) {
    //  hadPressed = 1;
    //  return k;
    //}
  }
  else if (PIND != 223) {
    //flashOn();
    //delay(200);
    switch (PINB)   {
      case 222: k = 45; break; //M
      case 221: k = 46; break; //,
      case 219: k = 47; break; //.
      case 215: k = 48; break; //Space
    }
  }

  else {
    hadPressed = 0;
    return 255;
  }
  //flashOff();
  if (k) {
    hadPressed = 1;
    return k;
  }
}


void loop() {
  unsigned char cmd;  // Vom Host gesendete Daten
  
  if(keyboard.available() || startup==0){
      while (keyboard.read(&cmd)) ;
      startup = keyboardcommand(cmd);//a=keyboard.keyboard_reply(cmd,0);
      if(startup) {pixels.setPixelColor(0, pixels.Color(30, 0, 30)); pixels.show(); delay(500);} //zeigt Initialisierung mit lila LED an
  }
  
  if (shiftPressed)   {
    _sym = 0; _fn = 0; idle = 0;
    //while (shiftPressed)delay(1);
    if (_shift == 0)     {
      delay(300);
      if (shiftPressed)       {
        _shift = 2;
        Mode = 2;
      } else  {
        _shift = 1;
        Mode = 1;
      }
    } else {
      delay(300);
      if (shiftPressed)       {
        if (_shift == 2)         {
          Mode = 0;
          _shift = 0;
        } else  {
          Mode = 2;
          _shift = 2;
        }
      } else  {
        Mode = 0;
        _shift = 0;
      }
    }
  }

  if (symPressed)   {
    _shift = 0; _fn = 0; idle = 0;
    if (_sym == 0)     {
      delay(300);
      if (symPressed)       {
        _sym = 2;
        Mode = 4;
      } else  {
        _sym = 1;
        Mode = 3;
      }
    } else {
      delay(300);
      if (symPressed)  {
        if (_sym == 2)  {
          Mode = 0;
          _sym = 0;
        } else  {
          Mode = 4;
          _sym = 2;
        }
      } else {
        Mode = 0;
        _sym = 0;
      }
    }
  }

  if (fnPressed) {
    _sym = 0; _shift = 0; idle = 0;
    if (_fn == 0) {
      delay(300);
      if (fnPressed) {
        _fn = 2;
        Mode = 6;
      } else  {
        _fn = 1;
        Mode = 5;
      }
    } else  {
      delay(300);
      if (fnPressed) {
        if (_fn == 2) {
          Mode = 0;
          _fn = 0;
        } else {
          Mode = 6;
          _fn = 2;
        }
      } else {
        Mode = 0;
        _fn = 0;
      }
    }
  }

  switch (Mode)  {
    case 0://normal
      pixels.setPixelColor(0, pixels.Color(0, 0, 0)); break;
    case 1://shift
      if ((idle / 6) % 2 == 1)
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      else
        pixels.setPixelColor(0, pixels.Color(5, 0, 0)); break;
    case 2://long_shift
      pixels.setPixelColor(0, pixels.Color(5, 0, 0)); break;
    case 3://sym
      if ((idle / 6) % 2 == 1)
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      else
        pixels.setPixelColor(0, pixels.Color(0, 5, 0)); break;
    case 4://long_sym
      pixels.setPixelColor(0, pixels.Color(0, 5, 0)); break;
    case 5://fn
      if ((idle / 6) % 2 == 1)
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      else
        pixels.setPixelColor(0, pixels.Color(0, 0, 5)); break;
    case 6://long_fn
      pixels.setPixelColor(0, pixels.Color(0, 0, 5)); break;
  }

  pixels.show(); // This sends the updated pixel color to the hardware.

  if (hadPressed == 0)   {
    KEY = GetInput();
  }

  // PS/2 Scancode senden
  if (hadPressed) {
    uint8_t c = KeyMap[KEY - 1][Mode];
    int16_t no = findScanMap(c);
    if (no >= 0) {
      flashOn();
      sendScanCode(no);
      if(old_no == no) delay(35);    //zwetes bis n.tes mal Tastenrepeat
      else delay(200);               //erstes mal Taste - 200ms Verzögerung
      flashOff();
      old_no = no;
      
    }

    if ((Mode == 1) || (Mode == 3) || (Mode == 5)) {
      Mode = 0;
      _shift = 0;
      _sym = 0;
      _fn = 0;
    }
    hadPressed = 0;

  }

  idle++;
  //delay(50);
}
