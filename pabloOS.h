#include "Presets.h"
// Include libraries for drawing to the OLED screen
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// Include libraries for the stepper motors
#include <AccelStepper.h>
#include <AFMotor.h>
#include <StatefulButton.h>
#include <RotaryEncoder.h>

// ==================================================================
// START: motor stuff

const int MOTOR_STEPS = 2048;

// wheel diameter of robot
const float WHEEL_DIAMETER = 79;

// Default configurations for stepper motor control
const long MAX_DISTANCE = 99999;
const int MAX_SPEED = 50;
const float ACCELERATION = 100;


// Declare the motors (for AFMotor lib)
AF_Stepper motorRight(MOTOR_STEPS, 1);
AF_Stepper motorLeft(MOTOR_STEPS, 2);

// These methods are used as 'wrappers' so that we can use 2 motor libraries together
// Note that each step can be SINGLE, DOUBLE, INTERLEAVE or MICROSTEP
void forwardStepRight() {
  motorRight.onestep(FORWARD, SINGLE);
}
void backwardStepRight() {
  motorRight.onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardStepLeft() {
  motorLeft.onestep(BACKWARD, SINGLE);
}
void backwardStepLeft() {
  motorLeft.onestep(FORWARD, SINGLE);
}

// Declare the AccelStepper motors (which 'wrap' the AFMotor lib motors)
AccelStepper stepperRight(forwardStepRight, backwardStepRight);
AccelStepper stepperLeft(forwardStepLeft, backwardStepLeft);

// These are settings captured by the UI settings
long setting_right_wheel_distance = 93;	// in mm
long setting_right_wheel_speed = 25;	// arbitrary scale
long setting_left_wheel_speed = 35;		// in mm
long setting_left_wheel_distance = 100;	// arbitrary scale



//  END:  motor stuff
// ==================================================================
// START: physical UI


// button pin positions
SimpleButton buttonIncThousands(37, true);		// K0: 22
SimpleButton buttonIncHundreds(35, true);		// K1: 23
SimpleButton buttonIncTens(33, true);		// K2: 24
SimpleButton buttonIncOnes(31, true);	 	// K3: 25

SimpleButton buttonDrawingMode(29, true);		// K4: 26
SimpleButton buttonUnused(27, true);			// K5: 27
SimpleButton buttonPresets(25, true);			// K6: 28
// ToggleButton buttonPresets(25, true, 9);			// K6: 28
// ToggleButton buttonPresets(25, true, sizeof (presets));			// K6: 28
SimpleButton buttonStart(23, true);			// K7: 29

// toggle button states
int buttonDrawingMode_state = 0;

// Many values are required for the action of the rotary controllers
//byte rotaryEncoderClkPin = 47;
//byte rotaryEncoderDtPin = 49;
RotaryEncoder encoder(49, 47);
SimpleButton rotaryEncoderBtn(45,true);
//int rotaryEncoderClkValue;
//int rotaryEncoderDtValue;
//int rotaryEncoderClkValuePrevious;

// Determines which of the 4 displayed values that the Rotary controller will modify
int rotaryMode = 0;
// holds the increment amount that the rotary controller uses
int rotaryIncrement = 1;
// current index of drawingMode
int drawingMode = 0;
// current index of preset array
int presetIndex = -1;



//  END:  physical UI
// ==================================================================
