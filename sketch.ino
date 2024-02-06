#include "SoftwareSerial.h"
#include <DFPlayer.h>
#include <EnergySaving.h>

int music_start_btn = 0;   // デジタルデータ出力用ピン番号
int state = 0;   // ピンより取得したデータ格納用

int voice_start_btn = 1;   // デジタルデータ出力用ピン番号
int voice_state = 0;   // ピンより取得したデータ格納用

#define MP3_RX_PIN              9     //GPIO4/D2 to DFPlayer Mini TX
#define MP3_TX_PIN              10     //GPIO5/D1 to DFPlayer Mini RX
#define MP3_SERIAL_SPEED        9600  //DFPlayer Mini suport only 9600-baud
#define MP3_SERIAL_BUFFER_SIZE  32    //software serial buffer size in bytes, to send 8-bytes you need 11-bytes buffer (start byte+8-data bytes+parity-byte+stop-byte=11-bytes)
#define MP3_SERIAL_TIMEOUT      350   //average DFPlayer response timeout for GD3200B chip 350msec..500msec

#define VOICE_RX_PIN            7     //GPIO4/D2 to DFPlayer Mini TX
#define VOICE_TX_PIN            8     //GPIO5/D1 to DFPlayer Mini RX
#define VOICE_SERIAL_TIMEOUT      350

SoftwareSerial mp3Serial(9, 10);
SoftwareSerial voiceSerial(7, 8);
DFPlayer       mp3;
DFPlayer       voice;

EnergySaving nrgSave;

void setup()
{
  Serial.begin(9600);
  mp3Serial.begin(9600);
  voiceSerial.begin(9600);

  nrgSave.begin(WAKE_EXT_INTERRUPT, music_start_btn, dummy);
  
  mp3.begin(mp3Serial, MP3_SERIAL_TIMEOUT, DFPLAYER_HW_247A, true); //"DFPLAYER_HW_247A" see NOTE, false=no feedback from module after the command
  voice.begin(voiceSerial, VOICE_SERIAL_TIMEOUT, DFPLAYER_HW_247A, true);

  mp3.setVolume(15);                      //0..30, module persists volume on power failure
  voice.setVolume(15);
                     
  Serial.println(mp3.getStatus());        //0=stop, 1=playing, 2=pause, 3=sleep or standby, 4=communication error, 5=unknown state
  Serial.println(voice.getStatus());

  pinMode(music_start_btn, INPUT);
  pinMode(voice_start_btn, INPUT);
}
                     
void loop() {
  state = digitalRead(music_start_btn);
  voice_state = digitalRead(voice_start_btn);
  Serial.println(state);
  int Jkboxrand = random(1, 13);
  if (state == 1) {
    mp3.stop();
    play(Jkboxrand);
  }
  int v_rand = random(1, 20);
  if (voice_state == 1) {
    voice.stop();
    voiceplay(v_rand);
  }

  nrgSave.standby();

}

void play(int num) {
  mp3.playTrack(num);
//  delay(21200);
}

void voiceplay(int num) {
  voice.playTrack(num);
//  delay(21200);
}

void dummy(void)  //interrupt routine (isn't necessary to execute any tasks in this routine
{

}
