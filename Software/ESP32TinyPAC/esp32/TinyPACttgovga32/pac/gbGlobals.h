#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 
 #include "gbConfig.h"
 #include <stdio.h>
 #include <Arduino.h>
 
 //extern FILE *gb_file_log;

 #ifdef use_lib_sound_dac
  extern volatile unsigned char gb_dac_vol;
 #endif 

 #ifdef use_lib_sound_digital
  extern hw_timer_t *gb_timerSound;
  extern volatile unsigned char gbVolMixer_now[3];
  extern volatile unsigned int gbFrecMixer_now[3];
  extern volatile unsigned short int gb_ct[3];
  extern volatile unsigned short int gb_ct_Pulse[3];  
  extern volatile unsigned char gb_flip[3];
  extern volatile unsigned char gb_sillence_all;
  extern volatile unsigned char gb_spk_data;
  extern volatile unsigned char gb_spk_data_before; 
  extern unsigned char gb_use_sound_digital;
  extern unsigned char gb_mute;
  extern unsigned char gb_sound_signed;
 #endif

 //#ifdef use_lib_sound_ay8912 
 // #include "fabgl.h" //Para fabgl
 // #include "fabutils.h" //Para fabgl
 // extern unsigned char gb_sillence_all;
 // extern SineWaveformGenerator gb_sineArray[3];
 // extern unsigned char gb_silence_all_channels;
 // extern unsigned char gbShiftLeftVolumen;
 // extern unsigned char gb_mute_sound;
 //
 // extern unsigned char gbVolMixer_before[3];
 // extern unsigned short int gbFrecMixer_before[3];
 // extern unsigned char gbVolMixer_now[3];
 // extern unsigned short int gbFrecMixer_now[3];
 //#endif 


 extern unsigned char gb_speed;

 extern const unsigned char * gb_sdl_font;
 
 extern unsigned char gb_end_setup;
 extern unsigned char gb_show_fps;

 extern volatile unsigned char keymap[256];
 extern volatile unsigned char oldKeymap[256];

 extern unsigned char gb_keyboard_cur_poll_ms; 
 extern unsigned char gb_vga_cur_poll_ms;
 extern unsigned char gb_reset;
 extern unsigned char gb_show_osd_main_menu;  

 //extern uint8_t* gb_pac_ptr_rom;
 extern uint8_t* gb_pac_ptr_ram;
 extern uint8_t* gb_pac_ptr_tiles;
 extern uint8_t* gb_pac_ptr_sprites;
 //extern uint8_t* gb_pac_ptr_tile_rom; //No se necesita se tiene en FLASH convertido
 //extern uint8_t* gb_pac_ptr_sprite_rom; //No se necesita se tiene en Flash convertido
 extern const unsigned char* gb_pac_ptr_palette_rom;
 extern uint8_t gb_pac_sprite_pos[0x10];
 extern bool gb_pac_flip_screen;
 extern unsigned char gb_pac_p1_up;
 extern unsigned char gb_pac_p1_left;
 extern unsigned char gb_pac_p1_right;
 extern unsigned char gb_pac_p1_down;
 extern unsigned char gb_pac_coin_s1;
 extern unsigned char gb_pac_coin_s2;
 extern unsigned char gb_pac_rack_advance;
 extern unsigned char gb_pac_credits_btn;
 extern unsigned char gb_pac_board_test;
 extern unsigned char gb_pac_p1_start;
 extern unsigned char gb_pac_p2_start;
 extern uint8_t gb_pac_int_vector;
 extern bool gb_pac_vblank_enabled;
 extern bool gb_pac_sound_enabled;
 extern uint32_t gb_wsg_voice_frequency[3];
 extern uint32_t gb_wsg_voice_accumulator[3];
 extern uint8_t gb_wsg_voice_waveform_no[3]; 
 extern uint8_t gb_wsg_voice_volume[3];
 extern const unsigned char* gb_ptr_id_rom[4]; //4 punteros a la rom 0x4000
 extern const unsigned char *gb_ptr_rom_82s1237f;


 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
  extern unsigned char gb_current_ms_poll_keyboard_uart;
 #endif

 //extern unsigned char gb_jj_screen[288*224];
 extern unsigned char gb_jj_color[32];

 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32; 
 extern unsigned char gb_sync_bits;
 extern const unsigned char pin_config[];
 extern const int *gb_ptrVideo_cur;
 //extern unsigned char gb_color_vga[256]; //revisar no lo necesito
 extern unsigned char * gb_224x288[288]; //doble buffer
 extern unsigned short int gb_ancho;
 extern unsigned short int gb_alto;
 extern unsigned char gb_ini_x;
 extern unsigned char gb_ini_y;
 extern unsigned char gb_vertical;
 extern unsigned char gb_videomode_cur;
 //extern unsigned char gb_videomode_last;
 
 extern unsigned int gb_fps_cur;
 extern unsigned int gb_fps_last;
 extern unsigned int gb_fps_min;
 extern unsigned int gb_fps_max;
 extern unsigned int gb_fps_med;
 extern unsigned int gb_fps_time_cur;
 extern unsigned int gb_fps_time_last;
 extern unsigned int gb_fps_real_vga;
 
 extern unsigned int gb_keyboard_time_cur;
 extern unsigned int gb_keyboard_time_last;  
 
 extern unsigned int gb_vga_time_last;
 extern unsigned int gb_vga_time_cur;
 
#endif

