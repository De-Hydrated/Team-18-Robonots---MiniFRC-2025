#include <Alfredo_NoU3.h>
#include <PestoLink-Receive.h>
#include <constants.h>
#include <thread>
#include <chrono> 
#include <cmath>
#include <string>

NoU_Motor backLeftDrive(4);
NoU_Motor frontLeftDrive(3);
NoU_Motor frontRightDrive(2);
NoU_Motor backRightDrive(1);
NoU_Motor endEffectorDrive(5);
NoU_Motor elevatorRightDrive(6);
NoU_Motor elevatorLeftDrive(7);

// Variables

// Motor powers for mecanum
double leftFrontPower = 0;
double rightFrontPower = 0;
double leftBackPower = 0;
double rightBackPower = 0;
float maxMotorPower;

// Elevator Positioning
int elevatorPosition;
int elevatorState = 0;
long elevatorTarget = 0;
float elevatorPower = 0;
float elevatorThrottle = 0;
bool elevatorUseSetpoint = true;

float elevatorIntegral = 0;
float elevatorPrevError = 0;


void setup() {
  
  NoU3.begin();

  PestoLink.begin("Robot part 2");

  backLeftDrive.set(0);
  frontLeftDrive.set(0);
  frontRightDrive.set(0);
  backRightDrive.set(0);

  elevatorRightDrive.beginEncoder();
}

void loop() {

  mecanumDrive();
  elevator();
  autonomous();
  intake();

  PestoLink.update();

}

// Mecanum Drive
void mecanumDrive() {

  double axial = -PestoLink.getAxis(1);  // These values should be adjusted accordingly during testing
  double lateral = -PestoLink.getAxis(0);
  double yaw = -PestoLink.getAxis(2);

  leftFrontPower = axial + lateral + yaw;
  rightFrontPower = axial - lateral - yaw;
  leftBackPower = axial - lateral + yaw;
  rightBackPower = axial + lateral - yaw;

  maxMotorPower = fmax(fabs(leftFrontPower), fabs(rightFrontPower));
  maxMotorPower = fmax(maxMotorPower, fabs(leftBackPower));
  maxMotorPower = fmax(maxMotorPower, fabs(rightBackPower));

  if (maxMotorPower > 1.0) {
    leftFrontPower /= maxMotorPower;
    rightFrontPower /= maxMotorPower;
    leftBackPower /= maxMotorPower;
    rightBackPower /= maxMotorPower;
  }

  backLeftDrive.set(leftBackPower);
  frontLeftDrive.set(leftFrontPower);
  frontRightDrive.set(rightFrontPower);
  backRightDrive.set(rightBackPower);

}
void autonomous() {

  if (PestoLink.buttonHeld(enableAuto)) { //run auto
    frontLeftDrive.set(1); // Drive Forward
    frontRightDrive.set(1);
    backLeftDrive.set(1);
    backRightDrive.set(1);
    delay(autoDriveTimeout);
    frontLeftDrive.set(0);
    frontRightDrive.set(0);
    backLeftDrive.set(0);
    backRightDrive.set(0);
    endEffectorDrive.set(1); // Outtake preload
    delay(autoOuttakeTimeout);
    endEffectorDrive.set(0);
  }
}

void elevator() {

  if (PestoLink.buttonHeld(toggleElevator)) {
    if (elevatorUseSetpoint == false) {
      elevatorUseSetpoint = true;
    }
    else {
      elevatorUseSetpoint = false;
    }
  }
  // Elevator position code
  if (PestoLink.buttonHeld(l1Button) && elevatorUseSetpoint) {
    elevatorTarget = l1Setpoint;
  }
  else if (PestoLink.buttonHeld(l2Button) && elevatorUseSetpoint) {
    elevatorTarget = l2Setpoint;
  }
  else if (PestoLink.buttonHeld(l3Button) && elevatorUseSetpoint) {
    elevatorTarget = l3Setpoint;
  }

  // PID Code   
  if (elevatorUseSetpoint) {
    long currentPos = elevatorRightDrive.getPosition();
    long error = elevatorTarget - currentPos;

    elevatorIntegral += error;
    float derivative = error - elevatorPrevError;

    elevatorPower =  -1 * ((kP * error) + (kI * elevatorIntegral) + (kD * derivative));
    elevatorPower = constrain(elevatorPower, -0.9, 0.9);

    // Stop when near target
    if (abs(error) < 40) {
        elevatorPower = 0;
        elevatorIntegral = 0;
    }

    elevatorRightDrive.set(elevatorPower);
    elevatorLeftDrive.set(-elevatorPower);
    elevatorPrevError = error;

  } 
  else {
    elevatorRightDrive.set(elevatorThrottle);
    elevatorLeftDrive.set(-elevatorThrottle);
    elevatorIntegral = 0;
    elevatorPrevError = 0;
  }
}

void intake() {

  // Intake/Outtake code
  if (PestoLink.buttonHeld(outtakeButton)) { // Intake + Outake
    endEffectorDrive.set(1);
  }
  if (PestoLink.buttonHeld(reverseOuttakeButton)){ // Reverse intake
    endEffectorDrive.set(-1);
  }
}
