#include "dtmf-wav.h"
#include "XT_DAC_Audio.h"
//                                                                     10    11   12   13   14   15
char dtmf_key[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};
char readString[30];

XT_Wav_Class DTMFTone0(DTMF0);
XT_Wav_Class DTMFTone1(DTMF1);
XT_Wav_Class DTMFTone2(DTMF2);
XT_Wav_Class DTMFTone3(DTMF3);
XT_Wav_Class DTMFTone4(DTMF4);
XT_Wav_Class DTMFTone5(DTMF5);
XT_Wav_Class DTMFTone6(DTMF6);
XT_Wav_Class DTMFTone7(DTMF7);
XT_Wav_Class DTMFTone8(DTMF8);
XT_Wav_Class DTMFTone9(DTMF9);
XT_Wav_Class DTMFTone14(DTMF14);
XT_Wav_Class DTMFTone15(DTMF15);

XT_DAC_Audio_Class DacAudio(25,0);

XT_Sequence_Class seqAlarm; 
XT_Sequence_Class seqClearAlarm; 

XT_Sequence_Class seqRelay1; 
XT_Sequence_Class seqClearRelay1; 

XT_Sequence_Class seqRelay2; 
XT_Sequence_Class seqClearRelay2; 

XT_Sequence_Class seqRelay3; 
XT_Sequence_Class seqClearRelay3; 

XT_Sequence_Class seqRelay4; 
XT_Sequence_Class seqClearRelay4; 

void setup() {
  Serial.begin(115200);

  seqAlarm.AddPlayItem(&DTMFTone14);  
  seqAlarm.AddPlayItem(&DTMFTone3);
  seqAlarm.AddPlayItem(&DTMFTone2);
  seqAlarm.AddPlayItem(&DTMFTone5);
  seqAlarm.AddPlayItem(&DTMFTone6);
  seqAlarm.AddPlayItem(&DTMFTone6);
  seqAlarm.AddPlayItem(&DTMFTone1);  
  seqAlarm.AddPlayItem(&DTMFTone15);

  seqRelay1.AddPlayItem(&DTMFTone14);  
  seqRelay1.AddPlayItem(&DTMFTone3);
  seqRelay1.AddPlayItem(&DTMFTone2);
  seqRelay1.AddPlayItem(&DTMFTone5);
  seqRelay1.AddPlayItem(&DTMFTone6);
  seqRelay1.AddPlayItem(&DTMFTone1);
  seqRelay1.AddPlayItem(&DTMFTone1);  
  seqRelay1.AddPlayItem(&DTMFTone15);
  
  seqRelay2.AddPlayItem(&DTMFTone14);  
  seqRelay2.AddPlayItem(&DTMFTone3);
  seqRelay2.AddPlayItem(&DTMFTone2);
  seqRelay2.AddPlayItem(&DTMFTone5);
  seqRelay2.AddPlayItem(&DTMFTone6);
  seqRelay2.AddPlayItem(&DTMFTone2);
  seqRelay2.AddPlayItem(&DTMFTone1);  
  seqRelay2.AddPlayItem(&DTMFTone15);
  
  seqRelay3.AddPlayItem(&DTMFTone14);  
  seqRelay3.AddPlayItem(&DTMFTone3);
  seqRelay3.AddPlayItem(&DTMFTone2);
  seqRelay3.AddPlayItem(&DTMFTone5);
  seqRelay3.AddPlayItem(&DTMFTone6);
  seqRelay3.AddPlayItem(&DTMFTone3);
  seqRelay3.AddPlayItem(&DTMFTone1);  
  seqRelay3.AddPlayItem(&DTMFTone15);
  
  seqRelay4.AddPlayItem(&DTMFTone14);  
  seqRelay4.AddPlayItem(&DTMFTone3);
  seqRelay4.AddPlayItem(&DTMFTone2);
  seqRelay4.AddPlayItem(&DTMFTone5);
  seqRelay4.AddPlayItem(&DTMFTone6);
  seqRelay4.AddPlayItem(&DTMFTone4);
  seqRelay4.AddPlayItem(&DTMFTone1);  
  seqRelay4.AddPlayItem(&DTMFTone15);
  
  seqClearAlarm.AddPlayItem(&DTMFTone14);  
  seqClearAlarm.AddPlayItem(&DTMFTone3);
  seqClearAlarm.AddPlayItem(&DTMFTone2);
  seqClearAlarm.AddPlayItem(&DTMFTone5);
  seqClearAlarm.AddPlayItem(&DTMFTone6);
  seqClearAlarm.AddPlayItem(&DTMFTone6);
  seqClearAlarm.AddPlayItem(&DTMFTone0);  
  seqClearAlarm.AddPlayItem(&DTMFTone15);
    
  seqClearRelay1.AddPlayItem(&DTMFTone14);  
  seqClearRelay1.AddPlayItem(&DTMFTone3);
  seqClearRelay1.AddPlayItem(&DTMFTone2);
  seqClearRelay1.AddPlayItem(&DTMFTone5);
  seqClearRelay1.AddPlayItem(&DTMFTone6);
  seqClearRelay1.AddPlayItem(&DTMFTone1);
  seqClearRelay1.AddPlayItem(&DTMFTone0);  
  seqClearRelay1.AddPlayItem(&DTMFTone15);
  
  seqClearRelay2.AddPlayItem(&DTMFTone14);  
  seqClearRelay2.AddPlayItem(&DTMFTone3);
  seqClearRelay2.AddPlayItem(&DTMFTone2);
  seqClearRelay2.AddPlayItem(&DTMFTone5);
  seqClearRelay2.AddPlayItem(&DTMFTone6);
  seqClearRelay2.AddPlayItem(&DTMFTone2);
  seqClearRelay2.AddPlayItem(&DTMFTone0);  
  seqClearRelay2.AddPlayItem(&DTMFTone15);
  
  seqClearRelay3.AddPlayItem(&DTMFTone14);  
  seqClearRelay3.AddPlayItem(&DTMFTone3);
  seqClearRelay3.AddPlayItem(&DTMFTone2);
  seqClearRelay3.AddPlayItem(&DTMFTone5);
  seqClearRelay3.AddPlayItem(&DTMFTone6);
  seqClearRelay3.AddPlayItem(&DTMFTone3);
  seqClearRelay3.AddPlayItem(&DTMFTone0);  
  seqClearRelay3.AddPlayItem(&DTMFTone15);
  
  seqClearRelay4.AddPlayItem(&DTMFTone14);  
  seqClearRelay4.AddPlayItem(&DTMFTone3);
  seqClearRelay4.AddPlayItem(&DTMFTone2);
  seqClearRelay4.AddPlayItem(&DTMFTone5);
  seqClearRelay4.AddPlayItem(&DTMFTone6);
  seqClearRelay4.AddPlayItem(&DTMFTone4);
  seqClearRelay4.AddPlayItem(&DTMFTone0);  
  seqClearRelay4.AddPlayItem(&DTMFTone15); 
    
  /*Sequence.RepeatForever = true;
  DacAudio.Play(&Sequence);*/
  
}

void loop() { 
  int charsRead;
  
  DacAudio.FillBuffer();                // Fill the sound buffer with data
  //if(DTMFTone0.Playing==false)       // if not playing,
  //  DacAudio.Play(&DTMFTone0);       // play it, this will cause it to repeat and repeat...
  //Serial.println(DemoCounter++);        // Showing that the sound will play as well as your code running here.

  if (Serial.available()) {
    Serial.println("NGS-110: Cmd>");
    
    charsRead = Serial.readBytesUntil('\n', readString, sizeof(readString) - 1);
    readString[charsRead] = '\0';
    
    if(readString[0] == 'A' && readString[1] == 'L'){ Serial.println("ALARM"); DacAudio.Play(&seqAlarm); }
    else if(readString[0] == 'R' && readString[1] == '1'){ Serial.println("RELAY1"); DacAudio.Play(&seqRelay1); }
    else if(readString[0] == 'R' && readString[1] == '2'){ Serial.println("RELAY2"); DacAudio.Play(&seqRelay2); }
    else if(readString[0] == 'R' && readString[1] == '3'){ Serial.println("RELAY3"); DacAudio.Play(&seqRelay3); }
    else if(readString[0] == 'R' && readString[1] == '4'){ Serial.println("RELAY4"); DacAudio.Play(&seqRelay4); }
    else if(readString[0] == '!' && readString[1] == 'A' && readString[2] == 'L'){ Serial.println("!ALARM"); DacAudio.Play(&seqClearAlarm); }
    else if(readString[0] == '!' && readString[1] == 'R' && readString[2] == '1'){ Serial.println("!RELAY1"); DacAudio.Play(&seqClearRelay1); }
    else if(readString[0] == '!' && readString[1] == 'R' && readString[2] == '2'){ Serial.println("!RELAY2"); DacAudio.Play(&seqClearRelay2); }
    else if(readString[0] == '!' && readString[1] == 'R' && readString[2] == '3'){ Serial.println("!RELAY3"); DacAudio.Play(&seqClearRelay3); }
    else if(readString[0] == '!' && readString[1] == 'R' && readString[2] == '4'){ Serial.println("!RELAY4"); DacAudio.Play(&seqClearRelay4); }
  }
}
