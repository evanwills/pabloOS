/*
 * Handles the motors
 */


// making this into a constant to save recalculating it
// over and over again
const long STEP_FACTOR = (MOTOR_STEPS / (WHEEL_DIAMETER * PI) / 2);

// register the number of steps by processing the captured settings
long steps = 0;

// top speed for the left motor
float speedL;

// top speed for the right motor
float speedR;

/*
 * This method captures all the knob settings
 * and gives them to the steppers.
 */
void captureSettings() {
  // TODO: update step info
  // so if I make the the knob think in mm, then
  // 1 turn is  (wheelDiam * 3.1416)
  // So, if someone says 100mm, then how many turns is that?
  // turns = configuredDistance / (wheelDiam * 3.1416)
  // given that 1 turn is 2048 steps then
  // turns in steps = 2048 * configuredDistance / (wheelDiam * 3.1416)

  // RIGHT WHEEL
  stepperRight.setMaxSpeed(settingRightWheelSpeed * 10); // max 400
  stepperRight.setAcceleration(ACCELERATION);
  // calculate how many steps to go (here we divide by 2 because the bounce goes fowards and backwards)
  // STEP_FACTOR = (MOTOR_STEPS / (WHEEL_DIAMETER * Pi) / 2)
  steps = (settingRightWheelDistance * STEP_FACTOR);
  stepperRight.moveTo(steps);
  // message(String(steps) );

  // LEFT WHEEL
  stepperLeft.setMaxSpeed(settingLeftWheelSpeed * 10); // max 400
  stepperLeft.setAcceleration(ACCELERATION);
  // STEP_FACTOR = (MOTOR_STEPS / (WHEEL_DIAMETER * Pi) / 2)
  steps = (settingLeftWheelDistance * STEP_FACTOR);
  stepperLeft.moveTo(steps);
}

/*
 * Allows programing the Artbot by defining how far each wheel will move
 * Both wheels are moved that distance within the same time frame
 */
void turnWheelsMM(long distance_l, long distance_r, float top_speed){

  setWheelsMM(distance_l, distance_r, top_speed);

  // Could make the check "> 1" so that the infinitesimal stop is not perceivable
  while(stepperLeft.distanceToGo() != 0 || stepperRight.distanceToGo() != 0){
    stepperLeft.run();
    stepperRight.run();
  }

  // reset the steppers to position 0
  stepperRight.setCurrentPosition(0);
  stepperLeft.setCurrentPosition(0);
}

void turnWheelsMM(long distance_l, long distance_r){
  turnWheelsMM( distance_l, distance_r, 300.0);
}

/*
 * Sets the wheels to turn without turning them
 */
void setWheelsMM(long distance_l, long distance_r,  float top_speed) {
  // calculate if there is a differential in the speeds
  if (abs(distance_l) >= abs(distance_r) ) {
    speedL = top_speed;
    speedR = top_speed * ((float)abs(distance_r) / (float)abs(distance_l));
  } else {
    speedR = top_speed;
    speedL = top_speed * ((float)abs(distance_l) / (float)abs(distance_r));
  }

  // translate distance into steps
  stepperLeft.setMaxSpeed(speedL);
  stepperLeft.setAcceleration(100000);
  stepperRight.setMaxSpeed(speedR);
  stepperRight.setAcceleration(100000);

  stepperLeft.moveTo(distanceToSteps(distance_l));
  stepperRight.moveTo(distanceToSteps(distance_r));
}

void setWheelsMM(long distance_l, long distance_r){
  setWheelsMM( distance_l, distance_r, 300.0);
}

// ===============================================
// This function is never called

/*
 * Allows finding out when wheels should be stopped.
 */
bool wheelsStillTurning(){
    // Could make the check "> 1" so that the infinitesimal stop is not perceivable
  if(stepperLeft.distanceToGo() != 0 || stepperRight.distanceToGo() != 0){
    stepperLeft.run();
    stepperRight.run();
    return true;
  } else {
    // reset the steppers to position 0
    stepperRight.setCurrentPosition(0);
    stepperLeft.setCurrentPosition(0);
    return false;
  }

}

/*
 * Converts distance covered by wheel into steps for stepper driver.
 */
long distanceToSteps(long mm) {
  return mm * 2048 / (WHEEL_DIAMETER * PI);
}

/*
 * Utility to stop steppers, and set them back to a 0 position
 */
void stopAndResetSteppers() {
  // stop everything
  stepperRight.stop(); // Stop as fast as possible: sets new target
  stepperLeft.stop(); // Stop as fast as possible: sets new target

  // set fast accelerations
  stepperLeft.setAcceleration(200);
  stepperRight.setAcceleration(200);

  // leep going until everything has stopper
  while(stepperLeft.distanceToGo() > 0 || stepperRight.distanceToGo() > 0){
    stepperLeft.run();
    stepperRight.run();
  }

  // reset the steppers to position 0
  stepperRight.setCurrentPosition(0);
  stepperLeft.setCurrentPosition(0);

  // tell the system that we are no longer drawing
  isDrawing = false;
  delay(1000);
}
