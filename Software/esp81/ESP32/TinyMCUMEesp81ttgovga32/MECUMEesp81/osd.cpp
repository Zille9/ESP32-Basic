#include "gbConfig.h"
#include "gbGlobals.h"
#include "hardware.h"
#include "MECUMEesp81.h"
#include "osd.h"
#include "emuapi.h"
#include "hardware.h"
#include "PS2Kbd.h"
#include "gb_sdl_font8x8.h"
//#include "dataFlash/gbpfile.h"


//#define BLACK   0
//#define BLUE    4
//#define RED     1
//#define MAGENTA 5
//#define GREEN   2
//#define CYAN    6
//#define YELLOW  3
//#define WHITE   15

//#ifdef COLOR_3B           //       BGR 
// #define BLACK   0x08      // 0000 1000
// #define BLUE    0x0C      // 0000 1100
// #define RED     0x09      // 0000 1001
// #define MAGENTA 0x0D      // 0000 1101
// #define GREEN   0x0A      // 0000 1010
// #define CYAN    0x0E      // 0000 1110
// #define YELLOW  0x0B      // 0000 1011
// #define WHITE   0x0F      // 0000 1111
//#endif
#include <SPI.h>
#include <SD.h>

SPIClass spiSD(HSPI);
#include <Update.h>

byte kSD_CS   = 13;
byte kSD_MISO = 16;
byte kSD_MOSI = 17;
byte kSD_CLK  = 14;



unsigned char gb_show_osd_main_menu=0;




//#define max_gb_osd_screen 1
//const char * gb_osd_screen[max_gb_osd_screen]={
// "Pixels Left"//,
// //"Pixels Top",
// //"Color 8",
// //"Mono Blue 8",
// //"Mono Green 8",
// //"Mono Red 8",
// //"Mono Grey 8"
//};

//#define max_gb_osd_screen_values 5
//const char * gb_osd_screen_values[max_gb_osd_screen_values]={
// "0",
// "2",
// "4", 
// "8", 
// "16"
//};


#define max_gb_main_menu 8
const char * gb_main_menu[max_gb_main_menu]={
 "Load Pfile",
 "Reset",
 "Delay CPU",
 "Video Poll",
 "Keyboard Poll",
 "Invert Color",
 "Return to Basic32+",
 "Return"
};


#define max_gb_delay_cpu_menu 15
const char * gb_delay_cpu_menu[max_gb_delay_cpu_menu]={
 "Auto",
 "0",
 "1",
 "2",
 "3",
 "4",
 "5",
 "6",
 "7",
 "8",
 "9",
 "10",
 "16",
 "20",
 "40"
};


//#define max_gb_speed_sound_menu 7
//const char * gb_speed_sound_menu[max_gb_speed_sound_menu]={
// "0",
// "1",
// "2",
// "4",
// "8",
// "16",
// "32"
//};

//#define max_gb_value_binary_menu 2
//const char * gb_value_binary_menu[max_gb_value_binary_menu]={
// "0",
// "1"
//};


//#define max_gb_speed_videoaudio_options_menu 2
//const char * gb_speed_videoaudio_options_menu[max_gb_speed_videoaudio_options_menu]={
// "Video poll",
// "Keyboard poll"
//};

#define max_gb_speed_video_poll_menu 6
const char * gb_speed_video_poll_menu[max_gb_speed_video_poll_menu]={
 "0",
 "10",
 "20",
 "30",
 "40",
 "50"
};

#define max_gb_speed_keyboard_poll_menu 5
const char * gb_speed_keyboard_poll_menu[max_gb_speed_keyboard_poll_menu]={
 "10",
 "20",
 "30",
 "40",
 "50"
};


//#define max_gb_speed_menu 5
//const char * gb_speed_menu[max_gb_speed_menu]={
// "Normal",
// "2x",
// "4x",
// "8x",
// "16x"
//};


//#define max_gb_osd_mouse_menu 2
//const char * gb_osd_mouse_menu[max_gb_osd_mouse_menu]={
// "right handed",
// "left handed"
//};

#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};


#define gb_pos_x_menu 60
#define gb_pos_y_menu 20
#define gb_osd_max_rows 10


//void jj_fast_putpixel(int x,int y,unsigned char c)
//{
// if ((x<0)||(x>319)||(y<0)||(y>199))
//  return;
//// vga.dotFast(x,y,gb_color_vga[c]);
// gb_buffer_vga[y][x^2]= gb_color_vga[c];
//}



#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart);        
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     Serial.printf("%02X ",gb_buf_uart[i]);
    }
    Serial.printf("\r\n"); 
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }

 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44        
        }
       }
      }
      else
      {       
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC              
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      break;

    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
#endif 


//***************************************************************************
inline void jj_fast_putpixel(short int x,short int y,unsigned char c)
{
 #ifdef use_lib_tinybitluni_fast
  gb_buffer_vga[y][x^2]= gb_color_vga[c];
 #else
  #ifdef use_lib_cvbs_bitluni   
   gb_buffer_cvbs[y][x]= gb_color_cvbs[(c & 0x01)];
  #endif
 #endif 
}


#ifdef use_lib_cvbs_bitluni
 //***********************************************************
 void SDLClearCVBS()
 {
  //#define gb_topeX 320
  //#define gb_topeY 200
  //#ifdef use_lib_video_2bpp
  // //DIV 4 2 bpp
  // #define gb_topeX_div4 20  
  //#else
  // #define gb_topeX_div4 80  
  //#endif 

  //unsigned int a32= gb_const_colorNormal[0];
  //unsigned int a32= 0;
  //a32= a32|(a32<<8)|(a32<<16)|(a32<<24);
  for (int y=0; y<200; y++)
  {
   for (int x=0; x<80; x++)
   {       
    gb_buffer_cvbs32[y][x]= 0;
   }
  }
 }

 //**********************************************
 void SDLSetBorderCVBS()
 {
  unsigned char aCol=1;
  if (gb_invert_color == 1){
   aCol= (~aCol)&0x01;
  }

  unsigned int a32= gb_color_cvbs[aCol];  
  a32= a32|(a32<<8)|(a32<<16)|(a32<<24);
  
  #ifdef use_lib_cvbs_bitluni_border
   //Si activamos border rellena todo blanco
   for (int y=0; y<200; y++)
   {
    for (int x=0; x<80; x++)
    {       
     gb_buffer_cvbs32[y][x]= a32;
    }
   }  
  #else
   //No queremos borde
   unsigned int x=0;
   for (unsigned int y=0; y<200; y++)
   {
    x=0;
    for (unsigned char i=0;i<8;i++)
    {
     gb_buffer_cvbs32[y][x++]= 0;
    }
    for (unsigned char i=0;i<64;i++)
    {
     gb_buffer_cvbs32[y][x++]= a32;
    }    
    for (unsigned char i=0;i<8;i++)
    {
     gb_buffer_cvbs32[y][x++]= 0;
    }
   }
  #endif    
 }
#endif

//**********************************************
void SDLClear()
{
 #ifdef use_lib_cvbs_bitluni
  SDLClearCVBS();
 #else
  for (int y=0; y<200; y++)
  {
   #ifdef use_lib_vga360x200
    for (int x=0; x<360; x++)
   #else
    for (int x=0; x<320; x++)
   #endif
    {
     jj_fast_putpixel(x,y,0);
    }   
  } 
 #endif
}

//***************************************
void SDLSetBorder()
{
 unsigned char aCol=1;
 if (gb_invert_color == 1){
  aCol= (~aCol)&0x01;
 }

 #ifdef use_lib_cvbs_bitluni
  SDLSetBorderCVBS();
 #else
  for (int y=0; y<200; y++)
  {
   #ifdef use_lib_vga360x200
    for (int x=0; x<360; x++)
   #else
    for (int x=0; x<320; x++)
   #endif
    {
     jj_fast_putpixel(x,y,aCol);
    }   
  } 
 #endif
}


//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 int auxId = car << 3; //*8
 unsigned char aux;
 unsigned char auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  aux = gb_sdl_font_8x8[auxId + j];
  for (int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
  }
 }
}

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 for (int i=0;i<gb_osd_max_rows;i++)
  SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),0,0);
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;
  //SDLprintText(gb_osd_sdl_surface,ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);
  SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_BLACK:ID_COLOR_WHITE),((i==0)?ID_COLOR_WHITE:ID_COLOR_BLACK));
  currentId++;
 }     
}

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax,short int aSel)
{
 unsigned char aReturn=0;
 unsigned char salir=0;
 unsigned int curTime_keyboard;
 unsigned int curTime_keyboard_before;

 #ifdef use_lib_keyboard_uart
  unsigned int curTime_keyboard_uart;
  unsigned int curTime_keyboard_before_uart;
  curTime_keyboard_uart = curTime_keyboard_before_uart= millis();
 #endif
 
 curTime_keyboard= curTime_keyboard_before= millis();

 SDLClear();
 SDLprintText("MCUME ZX81 mod by Ackerman",gb_pos_x_menu-(4<<3),gb_pos_y_menu-16,ID_COLOR_WHITE,ID_COLOR_BLACK);
 //for (int i=0;i<20;i++) 
 for (int i=0;i<14;i++) 
  SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);

 aReturn = (aSel!=-1)?aSel:0;
 OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
 
 while (salir == 0)
 {
  //case SDLK_UP:
  curTime_keyboard = millis();
  if ((curTime_keyboard - curTime_keyboard_before) >= gb_keyboard_cur_poll_ms)
  {
   curTime_keyboard_before= curTime_keyboard;

   #ifdef use_lib_keyboard_uart
    curTime_keyboard_uart= curTime_keyboard;
    if ((curTime_keyboard_uart - curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
    {
     curTime_keyboard_before_uart = curTime_keyboard_uart;
     keyboard_uart_poll();
    
     if (checkKey_uart(KEY_CURSOR_LEFT)==1)
     {
      if (aReturn>10) aReturn-=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_RIGHT)==1)
     {
      if (aReturn<(aMax-10)) aReturn+=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
     }  
     if (checkKey_uart(KEY_CURSOR_UP)==1)
     {
      if (aReturn>0) aReturn--;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_DOWN)==1)
     {
      if (aReturn < (aMax-1)) aReturn++;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_ENTER)==1)
     {
      salir= 1;
     }
     if (checkKey_uart(KEY_ESC))
     {
      salir=1; aReturn= 255;    
     }
    }
   #endif


   if (checkAndCleanKey(KEY_CURSOR_LEFT))
   {
    if (aReturn>10) aReturn-=10;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
   }
   if (checkAndCleanKey(KEY_CURSOR_RIGHT))
   {
    if (aReturn<(aMax-10)) aReturn+=10;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
   }
          
   if (checkAndCleanKey(KEY_CURSOR_UP))
   {
    if (aReturn>0) aReturn--;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
   }
   if (checkAndCleanKey(KEY_CURSOR_DOWN))
   {
    if (aReturn < (aMax-1)) aReturn++;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
   }
   if (checkAndCleanKey(KEY_ENTER))
   {
    salir= 1;
   }
   //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
   if (checkAndCleanKey(KEY_ESC))
   {
    salir=1; aReturn= 255;    
   }
   //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
   //default: break;             
  }
 } 
 gb_show_osd_main_menu= 0;
 return aReturn;
}

//Menu Load Pfile
void ShowTinyLoadPMenu()
{
 unsigned char aSelNum;     
 //aSelNum = ShowTinyMenu("PFILE",gb_list_pfile_title,max_list_pfile,gb_id_cur_pfile);
 aSelNum = ShowTinyMenu("PFILE",gb_ptr_pfile_title,gb_max_pfile,gb_id_cur_pfile);
 if (aSelNum != 255)
 {
  gb_load_new_pfile = 1;
  gb_id_cur_pfile = aSelNum;
 }
}

//Menu ROM
void ShowTinyMachineMenu()
{
 //unsigned char aSelNum;
 //model = ShowTinyMenu("Machine",gb_machine_menu,max_gb_machine_menu);
 //loadroms2FlashModel();
 //switch (aSelNum)
 //{
 // case 0: model= 0; loadroms2FlashModel(0); break;
 // case 1: model= 1; loadroms2FlashModel(1); break;
 // case 2: model= 2; loadroms2FlashModel(2); break;
 // default: model= 0; loadroms2FlashModel(0); break;
 //}
 //vTaskDelay(2);
 //resetz80();
 //resetcrtc();
 //SDL_Delay(2);
}


//Menu resetear
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu,-1);   
 if (aSelNum==255)
 {
   return;
 }
 //loadroms2FlashModel();
 //resetz80();
 //resetcrtc(); 
 if (aSelNum == 1)
 {
  ESP.restart();
 }
 else
 {
   gb_load_new_pfile=1;
   gb_soft_reset=1;
 } 
}

// perform the actual update from a given stream
void performUpdate(Stream &updateSource, size_t updateSize) {

  if (Update.begin(updateSize, U_FLASH, 2, 1, " ")) {
    size_t written = Update.writeStream(updateSource);

    if (Update.end()) {
      SDLprintText("Starter loaded successfully, now Reboot", (30), (160), ID_COLOR_WHITE, ID_COLOR_BLACK); //Serial.println("OTA done!");
      if (Update.isFinished()) {
        delay(1000);
        ESP.restart();
      }
    }
  }
}
//*********************************
void basic_loader(void) {

  spiSD.begin(kSD_CLK, kSD_MISO, kSD_MOSI, kSD_CS);         //SCK,MISO,MOSI,SS 13 //HSPI1

  if ( !SD.begin( kSD_CS, spiSD )) {                        //SD-Card starten
    SDLprintText("SD-Card Mount-Error!", (30), (160), ID_COLOR_WHITE, ID_COLOR_BLACK);
    return;
  }
  if ( !SD.exists("/run.bin") ) SDLprintText("Starter not found!", (30), (160), ID_COLOR_WHITE, ID_COLOR_BLACK);

  File updateBin = SD.open("/run.bin");

  if (updateBin) {
    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
      SDLprintText("load Starter", (30), (160), ID_COLOR_WHITE, ID_COLOR_BLACK);
      //Serial.println("load Basic32+");
      performUpdate(updateBin, updateSize);
    }

    updateBin.close();
  }

}



//Menu TAPE
/*void ShowTinyTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 load_tape2Flash(aSelNum);
}

//Play sound tape
void ShowTinySelectTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 Z80EmuSelectTape(aSelNum);
// gb_play_tape_current = aSelNum;
// gb_play_tape_on_now = 1;
}

//Menu SCREEN
void ShowTinySCRMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K SCREEN",gb_list_scr_48k_title,max_list_scr_48);
 load_scr2Flash(aSelNum);     
}
*/

//Menu velocidad emulador
void ShowTinySpeedMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Delay CPU",gb_delay_cpu_menu,max_gb_delay_cpu_menu,-1);
 if (aSelNum == 255)
 {
  return;
 }

 gb_auto_delay_cpu= (aSelNum==0)?1:0;//Auto delay
 switch (aSelNum)
 {
  case 0: break;
  case 1: gb_delay_tick_cpu_milis= 0; break;
  case 2: gb_delay_tick_cpu_milis= 1; break;
  case 3: gb_delay_tick_cpu_milis= 2; break;
  case 4: gb_delay_tick_cpu_milis= 3; break;
  case 5: gb_delay_tick_cpu_milis= 4; break;
  case 6: gb_delay_tick_cpu_milis= 5; break;
  case 7: gb_delay_tick_cpu_milis= 6; break;
  case 8: gb_delay_tick_cpu_milis= 7; break;
  case 9: gb_delay_tick_cpu_milis= 8; break;
  case 10: gb_delay_tick_cpu_milis= 9; break;
  case 11: gb_delay_tick_cpu_milis= 10; break;
  case 12: gb_delay_tick_cpu_milis= 16; break;
  case 13: gb_delay_tick_cpu_milis= 20; break;
  case 14: gb_delay_tick_cpu_milis= 30; break;  
  case 15: gb_delay_tick_cpu_milis= 40; break;  
 }
}

void ShowTinyKeyboardPollMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Poll ms Keyboard",gb_speed_keyboard_poll_menu,max_gb_speed_keyboard_poll_menu,-1);
 if (aSelNum==255)
 {
   return;
 }
 switch (aSelNum)
 {
  case 0: gb_keyboard_cur_poll_ms= 10; break;
  case 1: gb_keyboard_cur_poll_ms= 20; break;
  case 2: gb_keyboard_cur_poll_ms= 30; break;
  case 3: gb_keyboard_cur_poll_ms= 40; break;
  case 4: gb_keyboard_cur_poll_ms= 50; break;
 }
}

void ShowTinyVGAPollMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Poll ms VGA",gb_speed_video_poll_menu,max_gb_speed_video_poll_menu,-1);
 if (aSelNum==255)
 {
   return;
 }
 switch (aSelNum)
 {
  case 0: gb_blit_cur_poll_ms= 0; break;
  case 1: gb_blit_cur_poll_ms= 10; break;
  case 2: gb_blit_cur_poll_ms= 20; break;
  case 3: gb_blit_cur_poll_ms= 30; break;
  case 4: gb_blit_cur_poll_ms= 40; break;
  case 5: gb_blit_cur_poll_ms= 50; break;
 }
}



//Ajustar pantalla
//void ShowTinyScreenAdjustMenu()
//{
// unsigned char aSelNum, auxCol; 
// aSelNum= ShowTinyMenu("Screen Adjust",gb_osd_screen,max_gb_osd_screen);
// auxCol= ShowTinyMenu("Pixels",gb_osd_screen_values,max_gb_osd_screen_values);
// auxCol = auxCol<<1; //x2
 //gb_screen_xOffset = auxCol; 
 /*switch (aSelNum)
 {
  case 2:
   //OSD_PreparaPaleta8Colores();
   OSD_PreparaPaleta64para8Colores();
   memcpy(gb_cache_zxcolor,gb_const_colornormal8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_colornormal8_bright,8);
   #endif
   SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 3:
   OSD_PreparaPaleta64para8Colores();
   memcpy(gb_cache_zxcolor,gb_const_monoBlue8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoBlue8_bright,8);
   #endif 
   SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   //for (int k=0;k<256;k++)
   // printf("%d %d %d\n",gb_osd_sdl_palette[k].r,gb_osd_sdl_palette[k].g,gb_osd_sdl_palette[k].b);
   //for (int k=0;k<8;k++)
   // printf("%d ",gb_cache_zxcolor[k]);
   //printf("\n");    
   return;
   break;
  case 4:
   OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoGreen8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoGreen8_bright,8);
   #endif 
   SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 5:
   OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoRed8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoRed8_bright,8);
   #endif 
   SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
  case 6:
   OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_grey8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_grey8_bright,8);
   #endif 
   SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;          
 }
 auxCol= ShowTinyMenu("Cols or pixels",gb_osd_screen_values,max_gb_osd_screen_values);
 switch (aSelNum)
 {
  case 0: gb_screen_xIni = -(auxCol<<3); break;
  case 1: gb_screen_yIni = (auxCol<=3)? (-auxCol):(-3); break;  
  default: break;
 }
 */
//}

//void ShowTinyCOMMenu()
//{
// unsigned char aSelNum;     
// aSelNum = ShowTinyMenu("COM",gb_list_com_title,max_list_com);
//
// //gb_cartfilename= (char *)gb_list_rom_title[aSelNum];
// gb_force_load_com= 1;
// gb_id_cur_com= aSelNum;
// //running= 0;
//}

//void ShowTinyMouseMenu()
//{
// #ifdef use_lib_amx_mouse    
//  unsigned char aSelNum;
//  aSelNum = ShowTinyMenu("Mouse Buttons",gb_osd_mouse_menu,max_gb_osd_mouse_menu); 
//  switch (aSelNum)
//  {
//   case 0: gb_force_left_handed= 0; break; //diestro
//   case 1: gb_force_left_handed= 1; break; //zurdo
//   default: break;
//  }
// #endif
// vTaskDelay(2);
//}


//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= 1;   
}



//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
 if (checkAndCleanKey(KEY_F1))
 {
  gb_show_osd_main_menu= 1;
  return;
 }

 if (gb_show_osd_main_menu == 1)
 {
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu,-1);
  switch (aSelNum)
  {
   case 0:
    ShowTinyLoadPMenu();
    gb_show_osd_main_menu=0;
    break;
   case 1:
    ShowTinyResetMenu();
    gb_show_osd_main_menu=0;
    break;
   case 2: ShowTinySpeedMenu();
    gb_show_osd_main_menu=0;
    break;
   case 3: ShowTinyVGAPollMenu();
    gb_show_osd_main_menu=0;
    break;
   case 4: ShowTinyKeyboardPollMenu();
    gb_show_osd_main_menu=0;
    break;    
   case 5:
    gb_invert_color = (~gb_invert_color)&0x01; //invertir color
    break;
   case 6:
    basic_loader();
    break;
   default: break;
  }  
 }

 //SDLClear();
 SDLSetBorder(); //TRuco rapido borde color
 
 #ifdef use_lib_sound_ay8912
  gb_silence_all_channels = 0;
 #endif 
}
