/*This code controls the two front legs*/

#include <TMCStepper.h>
#include <AccelStepper.h>
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <SoftwareSerial.h>

// right front leg steppers
// knee stepper parameters
#define STEP_RK_DIR_PIN          22 // Direction
#define STEP_RK_STEP_PIN         23 // Step
#define STEP_RK_SW_SCK           24 // Software Slave Clock (SCK)
#define STEP_RK_SW_TX            25 // SoftwareSerial transmit pin
#define STEP_RK_SW_RX            26 // SoftwareSerial receive pin

// rotating stepper parameters
#define STEP_RR_DIR_PIN          27 // Direction
#define STEP_RR_STEP_PIN         28 // Step
#define STEP_RR_SW_SCK           29 // Software Slave Clock (SCK)
#define STEP_RR_SW_TX            30 // SoftwareSerial transmit pin
#define STEP_RR_SW_RX            31 // SoftwareSerial receive pin

// fixed parameters
#define STEP_RF_DIR_PIN          32 // Direction
#define STEP_RF_STEP_PIN         33 // Step
#define STEP_RF_SW_SCK           34 // Software Slave Clock (SCK)
#define STEP_RF_SW_TX            35 // SoftwareSerial transmit pin
#define STEP_RF_SW_RX            36 // SoftwareSerial receive pin

// left front leg steppers
// knee stepper parameters
#define STEP_LK_DIR_PIN          37 // Direction
#define STEP_LK_STEP_PIN         38 // Step
#define STEP_LK_SW_SCK           39 // Software Slave Clock (SCK)
#define STEP_LK_SW_TX            40 // SoftwareSerial transmit pin
#define STEP_LK_SW_RX            41 // SoftwareSerial receive pin

// rotating stepper parameters
#define STEP_LR_DIR_PIN          47 // Direction
#define STEP_LR_STEP_PIN         48 // Step
#define STEP_LR_SW_SCK           49 // Software Slave Clock (SCK)
#define STEP_LR_SW_TX            50 // SoftwareSerial transmit pin
#define STEP_LR_SW_RX            51 // SoftwareSerial receive pin

// fixed stepper parameters
#define STEP_LF_DIR_PIN          42 // Direction
#define STEP_LF_STEP_PIN         43 // Step
#define STEP_LF_SW_SCK           44 // Software Slave Clock (SCK)
#define STEP_LF_SW_TX            45 // SoftwareSerial transmit pin
#define STEP_LF_SW_RX            46 // SoftwareSerial receive pin

#define R_SENSE 0.11f // Internal Sense Resistor controls the max RMS current
#define stepsPerRevolution 200 // Motor steps per rev (1.8deg) // 80
#define minPos -1*stepsPerRevolution
#define maxPos stepsPerRevolution
int posInLK = 0;
int posInLR = 0;
int posInLF = 0;
int posInRK = 0;
int posInRR = 0;
int posInRF = 0;
                     
TMC2208Stepper driverRK = TMC2208Stepper(STEP_RK_SW_RX, STEP_RK_SW_TX, R_SENSE); // Contsruct driver1
AccelStepper stepperRK(AccelStepper::DRIVER, STEP_RK_STEP_PIN, STEP_RK_DIR_PIN); // Construct motor1

TMC2208Stepper driverRR = TMC2208Stepper(STEP_RR_SW_RX, STEP_RR_SW_TX, R_SENSE); // Contsruct driver2
AccelStepper stepperRR(AccelStepper::DRIVER, STEP_RR_STEP_PIN, STEP_RR_DIR_PIN); // Construct motor2

TMC2208Stepper driverRF = TMC2208Stepper(STEP_RF_SW_RX, STEP_RF_SW_TX, R_SENSE); // Contsruct driver3
AccelStepper stepperRF(AccelStepper::DRIVER, STEP_RF_STEP_PIN, STEP_RF_DIR_PIN); // Construct motor3

TMC2208Stepper driverLK = TMC2208Stepper(STEP_LK_SW_RX, STEP_LK_SW_TX, R_SENSE); // Contsruct driver1
AccelStepper stepperLK(AccelStepper::DRIVER, STEP_LK_STEP_PIN, STEP_LK_DIR_PIN); // Construct motor1

TMC2208Stepper driverLR = TMC2208Stepper(STEP_LR_SW_RX, STEP_LR_SW_TX, R_SENSE); // Contsruct driver2
AccelStepper stepperLR(AccelStepper::DRIVER, STEP_LR_STEP_PIN, STEP_LR_DIR_PIN); // Construct motor2

TMC2208Stepper driverLF = TMC2208Stepper(STEP_LF_SW_RX, STEP_LF_SW_TX, R_SENSE); // Contsruct driver3
AccelStepper stepperLF(AccelStepper::DRIVER, STEP_LF_STEP_PIN, STEP_LF_DIR_PIN); // Construct motor3

// Left leg encoders
// Encoder Knee
#define EncoderLK_PIN1 A0
#define EncoderLK_PIN2 A1

RotaryEncoder *encoderLK = nullptr;
void checkPositionLK()
{
  encoderLK->tick(); // just call tick() to check the state.
}

// Encoder Rotating
#define EncoderLR_PIN1 A2
#define EncoderLR_PIN2 A3

RotaryEncoder *encoderLR = nullptr;
void checkPositionLR()
{
  encoderLR->tick(); // just call tick() to check the state.
}

// Encoder Fixed
#define EncoderLF_PIN1 A4
#define EncoderLF_PIN2 A5

RotaryEncoder *encoderLF = nullptr;
void checkPositionLF()
{
  encoderLF->tick(); // just call tick() to check the state.
}

// Right leg encoders
// Encoder Knee
#define EncoderRK_PIN1 A6
#define EncoderRK_PIN2 A7

RotaryEncoder *encoderRK = nullptr;
void checkPositionRK()
{
  encoderRK->tick(); // just call tick() to check the state.
}

// Encoder Rotating
#define EncoderRR_PIN1 A8
#define EncoderRR_PIN2 A9

RotaryEncoder *encoderRR = nullptr;
void checkPositionRR()
{
  encoderRR->tick(); // just call tick() to check the state.
}

// Encoder Fixed
#define EncoderRF_PIN1 A10
#define EncoderRF_PIN2 A11

RotaryEncoder *encoderRF = nullptr;
void checkPositionRF()
{
  encoderRF->tick(); // just call tick() to check the state.
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Start...");

  // Left leg stepper
  // Stepper knee settings
  driverLK.begin();             // Initiate pins and registeries
  driverLK.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverLK.pwm_autoscale(true);
  driverLK.microsteps(0);
  stepperLK.setMaxSpeed(4000);
  stepperLK.setAcceleration(80000);
  stepperLK.setPinsInverted(false, false, true); // Invert Enable pin
  stepperLK.enableOutputs();

  // Stepper rotating settings
  driverLR.begin();             // Initiate pins and registeries
  driverLR.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverLR.pwm_autoscale(true);
  driverLR.microsteps(0);
  stepperLR.setMaxSpeed(4000);
  stepperLR.setAcceleration(80000);
  stepperLR.setPinsInverted(false, false, true); // Invert Enable pin
  stepperLR.enableOutputs();

  // Stepper fixed settings
  driverLF.begin();             // Initiate pins and registeries
  driverLF.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverLF.pwm_autoscale(true);
  driverLF.microsteps(0);
  stepperLF.setMaxSpeed(4000);
  stepperLF.setAcceleration(80000);
  stepperLF.setPinsInverted(false, false, true); // Invert Enable pin
  stepperLF.enableOutputs();

  // Right leg stepper
  // Stepper knee settings
  driverRK.begin();             // Initiate pins and registeries
  driverRK.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverRK.pwm_autoscale(true);
  driverRK.microsteps(0);
  stepperRK.setMaxSpeed(4000);
  stepperRK.setAcceleration(80000);
  stepperRK.setPinsInverted(false, false, true); // Invert Enable pin
  stepperRK.enableOutputs();

  // Stepper rotating settings
  driverRR.begin();             // Initiate pins and registeries
  driverRR.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverRR.pwm_autoscale(true);
  driverRR.microsteps(0);
  stepperRR.setMaxSpeed(4000);
  stepperRR.setAcceleration(80000);
  stepperRR.setPinsInverted(false, false, true); // Invert Enable pin
  stepperRR.enableOutputs();

  // Stepper fixed settings
  driverRF.begin();             // Initiate pins and registeries
  driverRF.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driverRF.pwm_autoscale(true);
  driverRF.microsteps(0);
  stepperRF.setMaxSpeed(4000);
  stepperRF.setAcceleration(80000);
  stepperRF.setPinsInverted(false, false, true); // Invert Enable pin
  stepperRF.enableOutputs();

  // Left leg encoders
  // Encoder knee settings
  encoderLK = new RotaryEncoder(EncoderLK_PIN1, EncoderLK_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderLK_PIN1), checkPositionLK, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderLK_PIN2), checkPositionLK, CHANGE);

  // Encoder rotating settings
  encoderLR = new RotaryEncoder(EncoderLR_PIN1, EncoderLR_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderLR_PIN1), checkPositionLR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderLR_PIN2), checkPositionLR, CHANGE);

  // Encoder fixed settings
  encoderLF = new RotaryEncoder(EncoderLF_PIN1, EncoderLF_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderLF_PIN1), checkPositionLF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderLF_PIN2), checkPositionLF, CHANGE);

  // Right leg encoders
  // Encoder knee settings
  encoderRK = new RotaryEncoder(EncoderRK_PIN1, EncoderRK_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderRK_PIN1), checkPositionRK, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRK_PIN2), checkPositionRK, CHANGE);

  // Encoder rotating settings
  encoderRR = new RotaryEncoder(EncoderRR_PIN1, EncoderRR_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderRR_PIN1), checkPositionRR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRR_PIN2), checkPositionRR, CHANGE);

  // Encoder fixed settings
  encoderRF = new RotaryEncoder(EncoderRF_PIN1, EncoderRF_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(EncoderRF_PIN1), checkPositionRF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderRF_PIN2), checkPositionRF, CHANGE);
}

void loop()
{
  int limit = 500;
  posInRK = map(analogRead(A13), 0, 1023, -limit, limit);
  posInRR = map(analogRead(A14), 0, 1023, -2000, 2000);
  posInRF = map(analogRead(A15), 0, 1023, -limit, limit);

  //Left leg Update
  //update encoderLK
  static int posLK = 0;
  encoderLK->tick();
  int newPosLK = ((encoderLK->getPosition())*(5.58));
  stepperLK.setCurrentPosition(newPosLK);
  //move stepperLK
  stepperLK.moveTo(posInLK);
  if(!inRange(stepperLK))
    stepperLK.run();

  //update encoderLR
  static int posLR = 0;
  encoderLR->tick();
  int newPosLR = ((encoderLR->getPosition())*(5.58));
  stepperLR.setCurrentPosition(newPosLR);
  //move stepperLR
  stepperLR.moveTo(posInLR);
  if(!inRange(stepperLR))
    stepperLR.run();

  //update encoderLF
  static int posLF = 0;
  encoderLF->tick();
  int newPosLF = ((encoderLF->getPosition())*(5.58));
  stepperLF.setCurrentPosition(newPosLF);
  //move stepperLF
  stepperLF.moveTo(posInLF);
  if(!inRange(stepperLF))
    stepperLF.run();

  //Right leg update
  //update encoderRK
  static int posRK = 0;
  encoderRK->tick();
  int newPosRK = ((encoderRK->getPosition())*(5.58));
  stepperRK.setCurrentPosition(newPosRK);
  //move stepperRK
  stepperRK.moveTo(posInRK);
  if(!inRange(stepperRK))
    stepperRK.run();

  //update encoderRR
  static int posRR = 0;
  encoderRR->tick();
  int newPosRR = ((encoderRR->getPosition())*(5.58));
  stepperRR.setCurrentPosition(newPosRR);
  //move stepperRR
  stepperRR.moveTo(posInRR);
  if(!inRange(stepperRR))
    stepperRR.run();

  //update encoderRF
  static int posRF = 0;
  encoderRF->tick();
  int newPosRF = ((encoderRF->getPosition())*(5.58));
  stepperRF.setCurrentPosition(newPosRF);
  //move stepperRF
  stepperRF.moveTo(posInRF);
  if(!inRange(stepperRF))
    stepperRF.run();

  Serial.print(stepperRK.distanceToGo());
  Serial.print(" ");
  Serial.print(stepperRR.distanceToGo());
  Serial.print(" ");
  Serial.println(stepperRF.distanceToGo());
}

/* This function increases stepper vibration
int encoderSet(RotaryEncoder *encoder, AccelStepper stepper, int pos)
{
  encoder->tick();
  int newPos = ((encoder->getPosition())*(5.58));
  if (pos != newPos)
    pos = newPos;
  stepper.setCurrentPosition(pos);
  return pos;
}
*/

bool inRange(AccelStepper stepper)
{
  return abs(stepper.distanceToGo())<=15;
}
