#include <TMCStepper.h>
#include <AccelStepper.h>
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <SoftwareSerial.h>

// Stepper1 parameters
#define STEP1_DIR_PIN          37 // Direction
#define STEP1_STEP_PIN         38 // Step
#define STEP1_SW_SCK           39 // Software Slave Clock (SCK)
#define STEP1_SW_TX            40 // SoftwareSerial transmit pin
#define STEP1_SW_RX            41 // SoftwareSerial receive pin

// Stepper2 parameters
#define STEP2_DIR_PIN          47 // Direction
#define STEP2_STEP_PIN         48 // Step
#define STEP2_SW_SCK           49 // Software Slave Clock (SCK)
#define STEP2_SW_TX            50 // SoftwareSerial transmit pin
#define STEP2_SW_RX            51 // SoftwareSerial receive pin

// Stepper3 parameters
//#define STEP3_EN_PIN           2 // Enable
#define STEP3_DIR_PIN          22 // Direction
#define STEP3_STEP_PIN         23 // Step
#define STEP3_SW_SCK           24 // Software Slave Clock (SCK)
#define STEP3_SW_TX            25 // SoftwareSerial transmit pin
#define STEP3_SW_RX            26 // SoftwareSerial receive pin
#define STEP3_SERIAL_PORT Serial3 // HardwareSerial port

#define R_SENSE 0.11f // Internal Sense Resistor controls the max RMS current
#define stepsPerRevolution 200 // Motor steps per rev (1.8deg) // 80
#define minPos -1*stepsPerRevolution
#define maxPos stepsPerRevolution
int posIn1 = 0;
int posIn2 = 0;
int posIn3 = 0;
                     
TMC2208Stepper driver1 = TMC2208Stepper(STEP1_SW_RX, STEP1_SW_TX, R_SENSE); // Contsruct driver1
AccelStepper stepper1(AccelStepper::DRIVER, STEP1_STEP_PIN, STEP1_DIR_PIN); // Construct motor1

TMC2208Stepper driver2 = TMC2208Stepper(STEP2_SW_RX, STEP2_SW_TX, R_SENSE); // Contsruct driver2
AccelStepper stepper2(AccelStepper::DRIVER, STEP2_STEP_PIN, STEP2_DIR_PIN); // Construct motor2

TMC2208Stepper driver3 = TMC2208Stepper(STEP3_SW_RX, STEP3_SW_TX, R_SENSE); // Contsruct driver3
AccelStepper stepper3(AccelStepper::DRIVER, STEP3_STEP_PIN, STEP3_DIR_PIN); // Construct motor3

// Encoder1
#define Encoder1_PIN1 A0
#define Encoder1_PIN2 A1

RotaryEncoder *encoder1 = nullptr;
void checkPosition1()
{
  encoder1->tick(); // just call tick() to check the state.
}

// Encoder2
#define Encoder2_PIN1 A2
#define Encoder2_PIN2 A3

RotaryEncoder *encoder2 = nullptr;
void checkPosition2()
{
  encoder2->tick(); // just call tick() to check the state.
}

// Encoder3
#define Encoder3_PIN1 A4
#define Encoder3_PIN2 A5

RotaryEncoder *encoder3 = nullptr;
void checkPosition3()
{
  encoder3->tick(); // just call tick() to check the state.
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Start...");

  // Stepper1 settings
  driver1.begin();             // Initiate pins and registeries
  driver1.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  //driver1.en_pwm_mode(1);      // Enable extremely quiet stepping
  driver1.pwm_autoscale(true);
  driver1.microsteps(0);
  stepper1.setMaxSpeed(4000);
  stepper1.setAcceleration(80000);
  //stepper1.setEnablePin(STEP1_EN_PIN);
  stepper1.setPinsInverted(false, false, true); // Invert Enable pin
  stepper1.enableOutputs();

  // Stepper2 settings
  driver2.begin();             // Initiate pins and registeries
  driver2.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  driver2.pwm_autoscale(true);
  driver2.microsteps(0);
  stepper2.setMaxSpeed(4000);
  stepper2.setAcceleration(80000);
  stepper2.setPinsInverted(false, false, true); // Invert Enable pin
  stepper2.enableOutputs();

  // Stepper3 settings
  driver3.begin();             // Initiate pins and registeries
  driver3.rms_current(1200);    // Set stepper current to 600mA. The command is the same as command driver.setCurrent(600, 0.11, 0.5);
  //driver3.en_pwm_mode(1);      // Enable extremely quiet stepping
  driver3.pwm_autoscale(true);
  driver3.microsteps(0);
  stepper3.setMaxSpeed(4000);
  stepper3.setAcceleration(80000);
  //stepper3.setEnablePin(STEP3_EN_PIN);
  stepper3.setPinsInverted(false, false, true); // Invert Enable pin
  stepper3.enableOutputs();

  // Encoder1 settings
  encoder1 = new RotaryEncoder(Encoder1_PIN1, Encoder1_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(Encoder1_PIN1), checkPosition1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Encoder1_PIN2), checkPosition1, CHANGE);

  // Encoder2 settings
  encoder2 = new RotaryEncoder(Encoder2_PIN1, Encoder2_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(Encoder2_PIN1), checkPosition2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Encoder2_PIN2), checkPosition2, CHANGE);

  // Encoder3 settings
  encoder3 = new RotaryEncoder(Encoder3_PIN1, Encoder3_PIN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(Encoder3_PIN1), checkPosition3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Encoder3_PIN2), checkPosition3, CHANGE);
}

void loop()
{
  posIn1 = map(analogRead(A13), 0, 1023, -500, 500);
  posIn2 = map(analogRead(A14), 0, 1023, -500, 500);
  posIn3 = map(analogRead(A15), 0, 1023, -500, 500);

  //update encoder1
  static int pos1 = 0;
  encoder1->tick();
  int newPos1 = ((encoder1->getPosition())*(5.58));
  stepper1.setCurrentPosition(newPos1);

  //move stepper 1
  stepper1.moveTo(posIn1);
  if(!inRange(stepper1))
    stepper1.run();

  //update encoder2
  static int pos2 = 0;
  encoder2->tick();
  int newPos2 = ((encoder2->getPosition())*(5.58));
  stepper2.setCurrentPosition(newPos2);

  //move stepper 2
  stepper2.moveTo(posIn2);
  if(!inRange(stepper2))
    stepper2.run();

  //update encoder3
  static int pos3 = 0;
  encoder3->tick();
  int newPos3 = ((encoder3->getPosition())*(5.58));
  stepper3.setCurrentPosition(newPos3);

  //move stepper 3
  stepper3.moveTo(posIn3);
  if(!inRange(stepper3))
    stepper3.run();

  Serial.print(stepper1.distanceToGo());
  Serial.print(" ");
  Serial.print(stepper2.distanceToGo());
  Serial.print(" ");
  Serial.println(stepper3.distanceToGo());
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
