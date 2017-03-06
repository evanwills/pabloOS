/*
  Handles the rotary controller
*/

void setupRotaryEncoder(){
  pinMode(rotaryEncoderClkPin, INPUT); // clk
  pinMode(rotaryEncoderDtPin, INPUT); // dt
  pinMode(rotaryEncoderBtnPin, INPUT_PULLUP); // btn
}

/*
 * This method reads all the rotary encoders
 */
void readRotaryEncoders() {
  rotaryEncoderClkValue = digitalRead(rotaryEncoderClkPin); // orange cable, CLK
  rotaryEncoderDtValue = digitalRead(rotaryEncoderDtPin);

  // rotaryMode; // Can be 0 1 2 or 3 (depending on which value we are changing)

  if ((rotaryEncoderClkValue != rotaryEncoderClkValuePrevious) && (rotaryEncoderClkValue == LOW)) { // Knob Rotated l when aVal changes, BUT use only if aVal is LOW.
    if (rotaryEncoderDtValue == LOW) {
      // INCREMENT
      if (rotaryMode == 0) {
        settingRightWheelDistance += rotaryIncrement;
        if (settingRightWheelDistance > 99999) settingRightWheelDistance = 99999;
      }
      else if (rotaryMode == 1) {
        settingRightWheelSpeed += 1;
        if (settingRightWheelSpeed > 50) settingRightWheelSpeed = 50;
      }
      else if (rotaryMode == 2) {
        settingLeftWheelSpeed += 1;
        if (settingLeftWheelSpeed > 50) settingLeftWheelSpeed = 50;
      }
      else {
        settingLeftWheelDistance += rotaryIncrement;
        if (settingLeftWheelDistance > 99999) settingLeftWheelDistance = 99999;
      }
      report();
    }
    else {
      // DECREMENT
      if (rotaryMode == 0) {
        settingRightWheelDistance -= rotaryIncrement;
      }
      else if (rotaryMode == 1) {
        settingRightWheelSpeed -= 1;
      }
      else if (rotaryMode == 2) {
        settingLeftWheelSpeed -= 1;
      }
      else {
        settingLeftWheelDistance -= rotaryIncrement;
      }
      report();
    }
  }

  rotaryEncoderClkValuePrevious = rotaryEncoderClkValue; // Don’t forget this
}

void cycleRotaryIncrement(){
  if (rotaryIncrement == 1) {
    rotaryIncrement = 10;
  }
  else if (rotaryIncrement == 10) {
    rotaryIncrement = 100;
  }
  else if (rotaryIncrement == 100) {
    rotaryIncrement = 1000;
  }
  else if (rotaryIncrement == 1000) {
    rotaryIncrement = 1;
  }
}
