/*
  MIDI note player

  This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
  If this circuit is connected to a MIDI synth, it will play the notes
  F#-0 (0x1E) to F#-5 (0x5A) in sequence.

  The circuit:
  - digital in 1 connected to MIDI jack pin 5
  - MIDI jack pin 2 connected to ground
  - MIDI jack pin 4 connected to +5V through 220 ohm resistor
  - Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

  created 13 Jun 2006
  modified 13 Aug 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Midi
*/
enum State
{
  BEGIN,
  SENSI,
  PLAY,
  INSENSI
};
const int MAX_V = 1024;
const int MIN_V = 600;
const int PERIOD = 100;
const int CMD_NOTE_ON = 0x90;
const int NOTE_MID_C = 60;
void setup()
{
  Serial.begin(115200);
  delay(10);
//   Serial.println("HELLO!");
}



State cstate = State::BEGIN;
unsigned long cur_time = 0;
int minv = MAX_V;
void loop()
{
  int sensor_val = analogRead(A0);
//   Serial.print("Current value : ");
//   Serial.println(sensor_val);
  switch (cstate){
    case State::BEGIN:
//   Serial.println("BEGIN");
    if (sensor_val < (MAX_V - 1)){
      //Start timer and switch to sensi state
      cur_time = millis();
      cstate = State::SENSI; //switch to the sensi state
    }
    break;
    case State::SENSI:
//   Serial.println("SENSI");

      minv = min(sensor_val,minv);
      //when timer expires switch to play state
      if ((millis() - cur_time )>= PERIOD){
        cstate = State::PLAY;
      }
    break;

    case State::PLAY:
//   Serial.println("PLAY");

      //play the note 
      //reset the timer and recorded value
      noteOn(CMD_NOTE_ON,NOTE_MID_C,minv);
      minv = MAX_V;
      cstate = State::INSENSI;
    break;
    case State::INSENSI:
//   Serial.println("INSENSI");

      //remain in this state untill the value goes back to max V
      if (sensor_val == MAX_V){
        cstate = State::BEGIN;
      }
  }
}

void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}