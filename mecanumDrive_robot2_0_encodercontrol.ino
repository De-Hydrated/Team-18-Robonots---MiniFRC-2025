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

bool autoDone = false;
bool lastAutoButton = false;

// Motor powers for mecanum
double leftFrontPower = 0;
double rightFrontPower = 0;
double leftBackPower = 0;
double rightBackPower = 0;
float maxMotorPower;

// Elevator Positioning
int elevatorState = 1;
long elevatorTarget = 0;
bool elevatorRunning = false;
int elevatorDirection = 1;
int elevatorPosition;
bool elevatorUseSetpoint = true;
bool lastToggle = false;


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

  bool current = PestoLink.buttonHeld(enableAuto); // Ensures that the autonomous is not repeatedly enabled when the button is pressed

  if (current && !lastAutoButton && !autoDone) { //run auto
    
    autoDone = true;

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

  lastAutoButton = current;

}

void elevator() {

  bool currentToggle = PestoLink.buttonHeld(toggleElevator); // Toggle the elevator

  if (currentToggle && !lastToggle) {
    elevatorUseSetpoint = !elevatorUseSetpoint;
  }

  lastToggle = currentToggle;

  // Elevator position and movement code
  elevatorPosition = elevatorRightDrive.getPosition();
  
  if (PestoLink.buttonHeld(l1Button) && elevatorUseSetpoint && elevatorState != 1) {
    elevatorTarget = l1Setpoint;
    elevatorRunning = true;
    elevatorState = 1;
  }
  else if (PestoLink.buttonHeld(l2Button) && elevatorUseSetpoint && elevatorState != 2) {
    elevatorTarget = l2Setpoint;
    elevatorRunning = true;
    elevatorState = 2;
  }
  else if (PestoLink.buttonHeld(l3Button) && elevatorUseSetpoint && elevatorState != 3) {
    elevatorTarget = l3Setpoint;
    elevatorRunning = true;
    elevatorState = 3;
  }

  if (!elevatorRunning && elevatorUseSetpoint) {
    setElevatorDirection(0);
  }
  else if (elevatorUseSetpoint) {
    // Simple P control (much safer than full power)
    int error = elevatorTarget - elevatorPosition;
    double power = error * 0.004;       // tune this
    power = std::clamp(power, -1.0, 1.0);

    setElevatorDirection(power);

    // stop when close enough
    if (abs(error) < 5) {  // acceptable tolerance
        setElevatorDirection(0);
        elevatorRunning = false;
    }
  }

  // Manual Elevator
  if (PestoLink.buttonHeld(elevatorUpButton) && !elevatorUseSetpoint) {
    setElevatorDirection(1);
  }
  else if (PestoLink.buttonHeld(elevatorDownButton) && !elevatorUseSetpoint) {
    setElevatorDirection(-1);
  }
}

void intake() {
  if (PestoLink.buttonHeld(outtakeButton)) { // Intake + Outake
    endEffectorDrive.set(1);
  }
  else if (PestoLink.buttonHeld(reverseOuttakeButton)){ // Reverse intake
    endEffectorDrive.set(-1);
  }
  else {
    endEffectorDrive.set(0);
  }
}


void setElevatorDirection(double direction) {
  elevatorRightDrive.set(direction);
  elevatorLeftDrive.set(-direction);
}