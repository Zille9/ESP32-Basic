 #include "gbConfig.h"
 #include "wsg.h" //revisar sonido
 #include "gbGlobals.h"

 //void wsg_init(wsg* const w, uint8_t* const sound_rom) //revisar sonido
 //void wsg_init(wsg* const w) //revisar sonido
 void wsg_init() //revisar sonido
 {  
  //for (int voice_no = 0; voice_no < 3; voice_no++){
  //  w->voices[voice_no].frequency = 0;
  //  w->voices[voice_no].accumulator = 0;
  //  w->voices[voice_no].waveform_no = 0;
  //  w->voices[voice_no].volume = 0;
  //}
  for (unsigned char voice_no = 0; voice_no < 3; voice_no++)
  {
    gb_wsg_voice_frequency[voice_no] = 0;
    gb_wsg_voice_accumulator[voice_no] = 0;
    gb_wsg_voice_waveform_no[voice_no] = 0;
    gb_wsg_voice_volume[voice_no] = 0;
  }  
  //w->sound_rom = sound_rom; //No lo necesito genero frecuencias con oscilador
  //w->gain = 25; //No lo necesito
 }

 //void wsg_write(wsg* const w, uint8_t address, uint8_t value)  //revisar sonido
 void wsg_write(uint8_t address, uint8_t value)  //revisar sonido
 {  
  // waveform 1
  if (address == 0x5) {
    //JJ w->voices[0].waveform_no = value & 0b111;
    gb_wsg_voice_waveform_no[0] = value & 0x07; //w->voices[0].waveform_no = value & 0x07;
  } else if (address >= 0x10 && address <= 0x14) {
    uint8_t sample_no = address - 0x10;
    gb_wsg_voice_frequency[0] &= ~(0x0F << (sample_no * 4)); //w->voices[0].frequency &= ~(0x0F << (sample_no * 4)); //JJ w->voices[0].frequency &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_frequency[0] |= (value & 0x0F) << (sample_no * 4); //w->voices[0].frequency |= (value & 0x0F) << (sample_no * 4); //JJ w->voices[0].frequency |= (value & 0b1111) << (sample_no * 4);
  } else if (address == 0x15) {
    gb_wsg_voice_volume[0] = value & 0xf; //w->voices[0].volume = value & 0xf;
  } else if (address >= 0 && address <= 0x4) {
    uint8_t sample_no = address - 0;
    gb_wsg_voice_accumulator[0] &= ~(0x0F << (sample_no * 4)); //w->voices[0].accumulator &= ~(0x0F << (sample_no * 4)); //JJ w->voices[0].accumulator &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_accumulator[0] |= (value & 0x0F) << (sample_no * 4); //w->voices[0].accumulator |= (value & 0x0F) << (sample_no * 4); //JJ w->voices[0].accumulator |= (value & 0b1111) << (sample_no * 4);
  }

  // waveform 2
  else if (address == 0xa) {
    gb_wsg_voice_waveform_no[1] = value & 0x07; //w->voices[1].waveform_no = value & 0x07; //JJ w->voices[1].waveform_no = value & 0b111;
  } else if (address >= 0x16 && address <= 0x19) {
    // voice 2 and 3 cannot set lowest 4 bits of frequency
    uint8_t sample_no = address - 0x16 + 1;
    gb_wsg_voice_frequency[1] &= ~(0x0F << (sample_no * 4)); //w->voices[1].frequency &= ~(0x0F << (sample_no * 4)); //JJ w->voices[1].frequency &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_frequency[1] |= (value & 0x0F) << (sample_no * 4); //w->voices[1].frequency |= (value & 0x0F) << (sample_no * 4);//JJ w->voices[1].frequency |= (value & 0b1111) << (sample_no * 4);
  } else if (address == 0x1a) {
    gb_wsg_voice_volume[1] = value & 0xf; //w->voices[1].volume = value & 0xf;
  } else if (address >= 0x6 && address <= 0x9) {
    // voice 2 and 3 cannot set lowest 4 bits of accumulator
    uint8_t sample_no = address - 0x6 + 1;
    gb_wsg_voice_accumulator[1] &= ~(0x0F << (sample_no * 4)); //w->voices[1].accumulator &= ~(0x0F << (sample_no * 4)); //JJ w->voices[1].accumulator &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_accumulator[1] |= (value & 0x0F) << (sample_no * 4); //w->voices[1].accumulator |= (value & 0x0F) << (sample_no * 4); //JJ w->voices[1].accumulator |= (value & 0b1111) << (sample_no * 4);
  }

  // waveform 3
  else if (address == 0xf) {
    gb_wsg_voice_waveform_no[2] = value & 0x07; //w->voices[2].waveform_no = value & 0x07; //JJ w->voices[2].waveform_no = value & 0b111;
  } else if (address >= 0x1b && address <= 0x1e) {
    // voice 2 and 3 cannot set lowest 4 bits of frequency
    uint8_t sample_no = address - 0x1b + 1;
    gb_wsg_voice_frequency[2] &= ~(0x0F << (sample_no * 4)); //w->voices[2].frequency &= ~(0x0F << (sample_no * 4)); //JJ w->voices[2].frequency &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_frequency[2] |= (value & 0x0F) << (sample_no * 4); //w->voices[2].frequency |= (value & 0x0F) << (sample_no * 4); //JJ w->voices[2].frequency |= (value & 0b1111) << (sample_no * 4);
  } else if (address == 0x1f) {
    gb_wsg_voice_volume[2] = value & 0xf; //w->voices[2].volume = value & 0xf;
  } else if (address >= 0xb && address <= 0xe) {
    // voice 2 and 3 cannot set lowest 4 bits of accumulator
    uint8_t sample_no = address - 0xb + 1;
    gb_wsg_voice_accumulator[2] &= ~(0x0F << (sample_no * 4)); //w->voices[2].accumulator &= ~(0x0F << (sample_no * 4)); //JJ w->voices[2].accumulator &= ~(0b1111 << (sample_no * 4));
    gb_wsg_voice_accumulator[2] |= (value & 0x0F) << (sample_no * 4); //w->voices[2].accumulator |= (value & 0x0F) << (sample_no * 4); //JJ w->voices[2].accumulator |= (value & 0b1111) << (sample_no * 4);
  }
  
 }

//JJ void wsg_play(wsg* const w, int16_t* const buffer, int buffer_len) //revisar sonido
//JJ {
  /*
  for (int i = 0; i < buffer_len; i++) {
    int16_t sample = 0;

    for (int voice_no = 0; voice_no < 3; voice_no++) {
      wsg_voice* const v = &w->voices[voice_no];

      if (v->frequency == 0 || v->volume == 0) {
        continue;
      }

      v->accumulator = (v->accumulator + v->frequency) & 0xfffff;

      // we use the highest 5 bits of the accumulator (which is
      // a 20 bit value) to select a sample (0-31) from a 32-step
      // waveform (1 step = 1 byte) in sound rom
      int sample_pos = v->waveform_no * 32 + (v->accumulator >> 15);

      // convert unsigned 8 bit sample to a signed 16 bit sample,
      // and multiply it by the volume of the voice
      int16_t voice_sample = (w->sound_rom[sample_pos] - 8) * v->volume;
      sample += voice_sample;
    }

    *(buffer + i) = sample * w->gain;
  }
  */
//JJ}
