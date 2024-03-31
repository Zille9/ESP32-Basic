//#######################################################################################################################################
// ------------------------------------- MP3 - Funktionalität ---------------------------------------------------------------------------
//#######################################################################################################################################
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
//#include "AudioGeneratorMOD.h"
#include "AudioOutputI2S.h"
//#include "AudioGeneratorMIDI.h"

AudioFileSourceID3 *id3;
AudioFileSourceSD *source = NULL;
//AudioGeneratorMOD *mod;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;

//#######################################################################################################################################
//--------------------------------------------- MP3 - Player ----------------------------------------------------------------------------
//#######################################################################################################################################


// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  String cbuf;
  int i = 0;
  const char *ptr = reinterpret_cast<const char *>(cbData);
  const char *tmpptr;
  tmpptr = ptr;

  (void) isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2) - 1] = 0;
  Terminal.printf("%s '%s' = '%s'\n\r", ptr, s1, s2);
  Terminal.flush();
}


void setGain(){
  float v = curGain / 100.0;
  out->SetGain(v);  //the parameter is the loudness as percent
}

void mp3_run(void)
{ while (!break_marker)           //Abbruch mit ESC
  {
    if (mp3->isRunning()) {
      if (!mp3->loop()) {
        mp3->stop();
        break_marker = true;      //am Song-Ende Ausstieg
      }
    }
    if (Key_u || Key_d) {         //+ und - f.laut und leiser
       Key_u = Key_d = 0;
       setGain();
    }
  }
}
/*
void mod_run(void)
{ while (!break_marker)           //Abbruch mit ESC
  {
    if (mod->isRunning()) {
      if (!mod->loop()) {
        mod->stop();
        break_marker = true;      //am Song-Ende Ausstieg
      }
    }
    if (Key_u || Key_d) {         //+ und - f.laut und leiser
       Key_u = Key_d = 0;
       setGain();
    }
  }
}


void play_mod() {
  
  fp = SD.open(String(sd_pfad) + String(tempstring));     //Datei zum Laden öffnen
  source = new AudioFileSourceSD();                       //SD-Karte als Quelle
  source->open(fp.name());                                //Datei ist Datenquelle
  out = new AudioOutputI2S(0, 1);                         //use the internal DAC channel 1 (pin25) on ESP32
  mod = new AudioGeneratorMOD();                          //Mod Codec-Generator starten
  mod->SetBufferSize(8*1024);
  mod->SetSampleRate(22050);
  mod->SetStereoSeparation(32);
  mod->begin(source, out);                               //midi starten
  setGain();                                              //Lautstärke einstellen
  mod_run();                                             //Playerschleife starten
  mod->stop();                                           //midi Stop
  source->close();                                        //Quelle schliessen
  break_marker = false;                                   //Abbruch_marker zurücksetzen
  sd_ende();                                              //SD-Karte beenden
  line_terminator();                                      //Leerzeile
}
*/
void play_mp3() {
  fp = SD.open(String(sd_pfad) + String(tempstring));     //Datei zum Laden öffnen
  source = new AudioFileSourceSD();                       //SD-Karte als Quelle
  out = new AudioOutputI2S(0, 1);                         //use the internal DAC channel 1 (pin25) on ESP32
  source->open(fp.name());                                //Datei ist Datenquelle
  id3 = new AudioFileSourceID3(source);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3");      //MP3-ID3-Infos lesen
  mp3 = new AudioGeneratorMP3();                          //MP3 Codec-Generator starten
  mp3->begin(id3, out);                                   //MP3 starten
  setGain();                                              //Lautstärke einstellen
  mp3_run();                                              //Playerschleife starten
  mp3->stop();                                            //MP3 Stop
  source->close();                                        //Quelle schliessen
  break_marker = false;                                   //Abbruch_marker zurücksetzen
  sd_ende();                                              //SD-Karte beenden
  line_terminator();                                      //Leerzeile
}
