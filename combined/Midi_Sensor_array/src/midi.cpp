#include"midi.h"
#include "constants.h"
#include <Arduino.h>

void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
