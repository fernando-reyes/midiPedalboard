#include <math.h>


int buttons[5] = {HIGH,HIGH,HIGH,HIGH,HIGH};
int hit         =  0;
int hihatSegment=  16;
int hhit        =  0; 
int hlastPos;

#define NBUT  5
#define MIDICHANNEL   9
#define HCUTOFF       100
#define HMCUTOFF      700

void setup() {
  for( int i=0; i<NBUT; i++){
    pinMode(i+2,INPUT_PULLUP);
//    digitalWrite(i+2,HIGH);
  }
    
  pinMode(13,OUTPUT);
  digitalWrite( 13, HIGH );

  pinMode(A0,INPUT_PULLUP);
  Serial.begin(31250);
}

void loop() {
  int newVal;
  for( int i=0; i<NBUT; i++){
    if( !( (newVal=digitalRead(i+2)) == buttons[i] ) ){
      if( (buttons[i]=newVal)==HIGH ){
        MIDI_TX( 144, 57+i , 0);
      }else{
        MIDI_TX( 144, 57+i , 127);
      }
      delay(100);
    }
  }

   if(( (hhit = analogRead(A0)) >= HCUTOFF ) && ( hhit <= HMCUTOFF ) &&
      ( (hhit = map( hhit , HCUTOFF, HMCUTOFF,127,0 )) >= 0 ) &&
      ( (hhit = (hihatSegment*int(hhit/hihatSegment) ) ) >= 0  ) &&
      ( hhit ==  hihatSegment*int(127/hihatSegment) ? hhit =  127 : 1 ) &&
      ( hhit == 16 ? (hhit=0,1) : 1 ) &&
      ( hhit != hlastPos ) )
      MIDI_TX(176,4, 127-(hlastPos = hhit));


}

void MIDI_TX(unsigned char CMD, unsigned char NOTE, unsigned char VELOCITY) {
  Serial.write(CMD|MIDICHANNEL);
  Serial.write(NOTE);
  Serial.write(VELOCITY);
}
