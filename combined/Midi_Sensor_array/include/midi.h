#pragma once
#include "constants.h"
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
void noteOn(int , int , int );