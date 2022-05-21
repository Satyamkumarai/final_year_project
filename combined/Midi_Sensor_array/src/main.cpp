#include <Arduino.h>
#include "midi.h"
#include "amux.h"

// Arduino to mux select pins map
static const int mux_select_pins [MAX_BITS] = {14, 12 , 13 ,15};



// ============= MIDI ==========


// ------------------------AMUX --------------------------------------


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
  switch (cstate[count]){
    case State::BEGIN:
    if (sensor_val < (MAX_V - NOISE_OFFSET)){
      //Start timer and switch to sensi state
      cur_time[count] = millis();
      cstate[count] = State::SENSI; //switch to the sensi state
    }
    break;
    case State::SENSI:

      minv[count] = min(sensor_val,minv[count]);
      //when timer expires switch to play state
      if ((millis() - cur_time[count] )>= PERIOD){
        cstate[count] = State::PLAY;
      }
    break;

    case State::PLAY:
      //play the note 
      //reset the timer and recorded value
       vel[count] = map(minv[count] , MAX_V , MIN_V , 20,126);
      noteOn(CMD_NOTE_ON,NOTES[count],vel[count]);
      minv[count] = MAX_V;
      cstate[count] = State::INSENSI;
    break;
    case State::INSENSI:
      //remain in this state untill the value goes back to max V
      if (sensor_val == MAX_V){
        cstate[count] = State::BEGIN;
        noteOn(CMD_NOTE_OFF,NOTES[count],100);
      }
  }
}

}