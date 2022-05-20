#include <Arduino.h>
const int BAUD = 115200;
const int MAX_BITS = 4;
const int MAX_NUM = 2;

// Arduino to mux select pins map
static const int mux_select_pins [MAX_BITS] = {14, 12 , 13 ,15};



// ============= MIDI ==========
enum State
{
  BEGIN,
  SENSI,
  PLAY,
  INSENSI
};

const int NOISE_OFFSET = 10;
const int MAX_V = 1024;
const int MIN_V = 700;
const int PERIOD = 100;
const int CMD_NOTE_ON = 0x90;
const int CMD_NOTE_OFF = 0x80;
// const int NOTE_MID_C = 60;
const int NOTES[MAX_NUM] = {60,63}; // C and E


// ------------------------AMUX --------------------------------------
int read_val( int select ){
	if (select > MAX_NUM){
		return BAUD;
	} 

	//Toggle SELECT 
	for (int bit = 0 ; bit < MAX_BITS; bit++){		
		// Serial.print(select );
		// Serial.print(" : ");
		// Serial.print( bit );
		// Serial.print("  ");
		// Serial.print(mux_select_pins[bit]);
		// Serial.print(" >> ");
		// Serial.println(((select>> bit )&1));
		digitalWrite(mux_select_pins[bit],(select >> bit) & 1);

	}
	return analogRead(A0);
	
}
// ------------------------AMUX  END--------------------------------------

// ------------------------MIDI--------------------------------------

void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

// ------------------------MIDI END --------------------------------------




int vals[MAX_NUM]  =  {0};

// ======MIDI=======


State cstate[MAX_NUM] = {State::BEGIN};
unsigned long cur_time[MAX_NUM] = {0};
int minv[MAX_NUM] = {MAX_V};
int vel[MAX_NUM]={0};










void setup() {
// ------------------------AMUX --------------------------------------
  
  Serial.begin(BAUD);
	//init Mux Select Pins 
	for (int i=0;i<MAX_NUM;i++)
		pinMode(mux_select_pins[i],OUTPUT);
// ------------------------AMUX  END--------------------------------------



}

void loop() {
for (int count = 0 ; count < MAX_NUM ; count++){


int sensor_val = read_val(count);
//   Serial.print("Current value : ");
//   Serial.println(sensor_val);
  switch (cstate[count]){
    case State::BEGIN:
//   Serial.println("BEGIN");
    if (sensor_val < (MAX_V - NOISE_OFFSET)){
      //Start timer and switch to sensi state
      cur_time[count] = millis();
      cstate[count] = State::SENSI; //switch to the sensi state
    }
    break;
    case State::SENSI:
//   Serial.println("SENSI");

      minv[count] = min(sensor_val,minv[count]);
      //when timer expires switch to play state
      if ((millis() - cur_time[count] )>= PERIOD){
        cstate[count] = State::PLAY;
      }
    break;

    case State::PLAY:
//   Serial.println("PLAY");

      //play the note 
      //reset the timer and recorded value
       vel[count] = map(minv[count] , MAX_V , MIN_V , 20,126);
      noteOn(CMD_NOTE_ON,NOTES[count],vel[count]);
      minv[count] = MAX_V;
      cstate[count] = State::INSENSI;
    break;
    case State::INSENSI:
//   Serial.println("INSENSI");

      //remain in this state untill the value goes back to max V
      if (sensor_val == MAX_V){
        cstate[count] = State::BEGIN;
        noteOn(CMD_NOTE_OFF,NOTES[count],100);
      }
  }
}

}