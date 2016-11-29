// Allows knowing if the steppers are currently activated or just waiting.
boolean isDrawing = false;

void setup()
{
  setupDisplay();
  setupRotaryEncoder();
  setupButtons();

  Serial.begin (9600);
}

void loop(){

  // check which mode we are
  if (drawingMode == 0) {
    default_mode();
  } else if ( drawingMode == 1) {
    customDrawingMode();
    report(); // this ensures that the screen re-displays what is going on in default mode
  }

}

void default_mode(){

  if (!isDrawing) {
    // Choose parameter. This controls which value will be modified by the knob
    if (rotaryEncoderBtn.isPressed() == true) {
      rotaryMode = (rotaryMode + 1) % 4;
      report();
      delay(300);
    }

    readRotaryEncoders();

    // Choose drawingMode
    readModeButton();

    // Change increment.
    if (buttonIncThousands.isPressed() == true) {
      rotaryIncrement = 1000; report();
      // cycleRotaryIncrement();
    } else if (buttonIncHundreds.isPressed() == true){
      rotaryIncrement = 100; report();
    } else if (buttonIncTens.isPressed() == true){
      rotaryIncrement = 10; report();
    } else if (buttonIncOnes.isPressed() == true){
      rotaryIncrement = 1; report();
    }

    // Choose preset
    if (buttonPresets.isPressed() == true) {
      // increment presetIndex
	//   presetIndex = buttonPresets.getState();
	  presetIndex = presetIndex + 1;

    //   check if at end of preset length, wrap
      if (presetIndex >= (sizeof (presets) / sizeof (*presets)) ){
        presetIndex = 0;
      }

      // make sure preset values are captured
      setting_left_wheel_distance = presets[presetIndex][0];
      setting_left_wheel_speed = presets[presetIndex][1];   // in mm
      setting_right_wheel_speed = presets[presetIndex][2];  // arbitrary scale
      setting_right_wheel_distance = presets[presetIndex][3]; // in mm

      report();
      delay(200);
    }

    // Push to start
    if (buttonStart.isPressed() == true) {
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
    if (buttonStart.isPressed() == true) {
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
  return (buttonStart.isPressed() == false);
}


void readModeButton(){
  if (buttonDrawingMode.isPressed() == true) {
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
