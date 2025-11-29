#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>
#include <thread>
#include <chrono> 
#include <cmath>

float servo_min = 1000;
float servo_max = 2000;
float autoTimeout = 15000;
float autoDriveTimeout = 10000;
float autoOuttakeTimeout = autoTimeout - autoDriveTimeout;

NoU_Motor backLeftDrive(4);
NoU_Motor frontLeftDrive(3);
NoU_Motor frontRightDrive(2);
NoU_Motor backRightDrive(1);
NoU_Motor eeDrive(5);
NoU_Motor elevatorRightDrive(6);
NoU_Motor elevatorLeftDrive(7);

double leftFrontPower = 0;
double rightFrontPower = 0;
double leftBackPower = 0;
double rightBackPower = 0;
double maxMotorPower;

float l2Timeout = 200;
float l3Timeout = 300;
float l4Timeout = 400;
float elevatorState = 0

void setup() {
  // put your setup code here, to run once:
  PestoLink.begin("Robot part 2");

  backLeftDrive.set(0);
  frontLeftDrive.set(0);
  frontRightDrive.set(0);
  backRightDrive.set(0);
}

void loop() {

  //Center Auto
  if (PestoLink.buttonHeld(13)) { //run auto
    //drive forward
    leftFrontDrive.set(1);
    rightFrontDrive.set(1);
    leftBackDrive.set(1);
    rightBackDrive.set(1);
    wait(autoDriveTimeout);
    leftFrontDrive.set(0);
    rightFrontDrive.set(0);
    leftBackDrive.set(0);
    rightBackDrive.set(0);
    //outtake preload
    eeDrive.set(1);
    wait(autoOuttakeTimeout);
    eeDrive.set(0);
  }

  // Mecanum code
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

  eeDrive.set(0);

  if (max > 1.0) {
    leftFrontPower /= maxMotorPower;
    rightFrontPower /= maxMotorPower;
    leftBackPower /= maxMotorPower;
    rightBackPower /= maxMotorPower;
  }

  backLeftDrive.set(leftBackPower);
  frontLeftDrive.set(leftFrontPower);
  frontRightDrive.set(rightFrontPower);
  backRightDrive.set(rightBackPower);

  // Elevator code
  if (PestoLink.buttonHeld(0)) { //l1/stow
    if (elevatorState == 0) { //stay at stow
      ;
    }
    else if (elevatorState == 2) { //l2 to stow
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 0;
    }
    else if (elevatorState == 3) { //l3 to stow
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l3Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 0;
    }
    else if (elevatorState == 4) { //l4 to stow
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l4Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 0;
    }
  }

  if (PestoLink.buttonHeld(2)) { //l2
    if (elevatorState == 0) { //stow to l2
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0);
      elevatorState = 2;
    }
    else if (elevatorState == 2) { //stay at l2
      ;
    }
    else if (elevatorState == 3) { //l3 to l2
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l3Timeout-l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 2;
    }
    else if (elevatorState == 4) { //l4 to l2
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l4Timeout-l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 2;
    }
  }

  if (PestoLink.buttonHeld(1)) { //l3
    if (elevatorState == 0) {
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l3Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 3;
    }
    else if (elevatorState == 2) { //l2 to l3
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l3Timeout-l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 3;
    }
    else if (elevatorState == 3) { //stay at l3
      ;
    }
    else if (elevatorState == 4) { //l3 to l4
      elevatorRightDrive.set(-1);
      elevatorLeftDrive.set(-1);
      wait(l4Timeout-l3Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 3;
    }
  }

  if (PestoLink.buttonHeld(3)) { //l4
    if (elevatorState == 0) { //stow to l4
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l4Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 4;
    }
    else if (elevatorState == 2) { //l2 to l4
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l4Timeout-l2Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 4;
    }
    else if (elevatorState == 3) { //l3 to l4
      elevatorRightDrive.set(1);
      elevatorLeftDrive.set(1);
      wait(l4Timeout-l3Timeout);
      elevatorRightDrive.set(0);
      elevatorLeftDrive.set(0;
      elevatorState = 4;
    }
    else if (elevatorState == 4) { //stay at l4
      ;
    }
  }

  //EE code
  if (PestoLink.buttonHeld(7)) { //Outake
    eeDrive.set(1);
  }
  if (PestoLink.buttonHeld(6)){ //Intake
    eeDrive.set(-1);
  }

  PestoLink.update();

}
