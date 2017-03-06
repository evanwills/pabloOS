// Allows knowing if the steppers are currently activated or just waiting.
boolean isDrawing = false;

void setup()
{
  setupDisplay();
  setupRotaryEncoder();
  setupButtons();
}

void loop(){

  // check which mode we are
  if (drawingMode == 0) {
    defaultMode();
  } else if ( drawingMode == 1) {
    customDrawingMode(); // see pabloOS.ino
    report(); // this ensures that the screen re-displays what is going on in default mode
  }

}

void defaultMode(){

  if (!isDrawing) {
    readRotaryEncoders();

    // Choose parameter. This controls which value will be modified by the knob
    if (digitalRead(rotaryEncoderBtnPin) == LOW) {
      rotaryMode = (rotaryMode + 1) % 4;
      report();
      delay(300);
    }

    // Choose drawingMode
    readModeButton();

    // Change increment.
    if (digitalRead(buttonIncThousands) == LOW) {
      rotaryIncrement = 1000; report();
      // cycleRotaryIncrement();
    } else if (digitalRead(buttonIncHundreds) == LOW){
      rotaryIncrement = 100; report();
    } else if (digitalRead(buttonIncTens) == LOW){
      rotaryIncrement = 10; report();
    } else if (digitalRead(buttonIncOnes) == LOW){
      rotaryIncrement = 1; report();
    }

    // Choose preset
    if (digitalRead(buttonPresets) == LOW) {
      // increment presetIndex
      presetIndex = presetIndex + 1;

      // check if at end of preset length, wrap
      if (presetIndex >= (sizeof (presets) / sizeof (*presets)) ){
        presetIndex = 0;
      }

      // make sure preset values are captured
      settingLeftWheelDistance = presets[presetIndex][0];
      settingLeftWheelSpeed = presets[presetIndex][1];   // in mm
      settingRightWheelSpeed = presets[presetIndex][2];  // arbitrary scale
      settingRightWheelDistance = presets[presetIndex][3]; // in mm

      report();
      delay(200);
    }

    // Push to start
    if (digitalRead(buttonStart) == LOW) {
      isDrawing = true;
      displayStartMessage();
      captureSettings();
      report();
    }

  } else {

    // Bounce the right wheel direction
    if (stepperRight.distanceToGo() == 0) {
      stepperRight.moveTo(-stepperRight.currentPosition());
    }

    // Bounce the left wheel direction
    if (stepperLeft.distanceToGo() == 0) {
      stepperLeft.moveTo(-stepperLeft.currentPosition());
    }

    // Look for stop button
    if (digitalRead(buttonStart) == LOW) {
      messageLarge("Stop!");
      stopAndResetSteppers();
      report();
    }

    // Go!
    stepperRight.run();
    stepperLeft.run();

  }
}

/*
 * Waits for start button to be pressed
 */
bool waitForStartButton(){
  return (digitalRead(buttonStart) == HIGH);
}


void readModeButton(){
  if ( digitalRead(buttonDrawingMode) == LOW ) {
    // mode button pressed

    if ( buttonDrawingMode_state == 0) {
      // if we get here then it is a fresh push

      buttonDrawingMode_state = 1;
      drawingMode ++;
      if (drawingMode > 1) drawingMode = 0;
    }

  } else {
    // mode button unpressed,
    buttonDrawingMode_state = 0;
  }
}
