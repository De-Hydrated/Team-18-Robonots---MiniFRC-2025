#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>
#include <thread>
#include <chrono>

NoU_Motor backLeftDrive(4);
NoU_Motor frontLeftDrive(3);
NoU_Motor frontRightDrive(2);
NoU_Motor backRightDrive(1);
NoU_Drivetrain drivetrain(&frontLeftDrive, &frontRightDrive, &backLeftDrive, &backRightDrive);

// Mechanism motors
NoU_Servo elevatorServoR(4, servo_min, servo_max);
NoU_Motor outtakeMotor(6);

//declare variables for servo pulse width max and min
float servo_min = 1000;
float servo_max = 2000;

//declare variable for outtake delay
float outtake_delay = 1.5;

void setup() {
  // put your setup code here, to run once:
  PestoLink.begin("Hello!"); // Change name

  backLeftDrive.set(0);
  frontLeftDrive.set(0);
  frontRightDrive.set(0);
  backRightDrive.set(0);
  outtakeMotor.setState(BRAKE);

}

void loop() {
  // put your main code here, to run repeatedly:
  float rotation = 0;
  float throttle = 0;

  rotation = 1 * PestoLink.getAxis(2);
  throttle = 1 * PestoLink.getAxis(1);

  throttle = throttle * -1;

  drivetrain.arcadeDrive(throttle, rotation);

// Once  gear ratio is put on setpoints need to be tested
  float l1_setpoint = 0;
  float l2_setpoint = 45;
  float l3_setpoint = 90; 
  float l4_setpoint = 180;
  float intake_setpoint = 135;

  //logic to drive elevator servo to setpoint angle
  //intaking coral
  if (PestoLink.buttonHeld(10)) { // Rebind
    elevatorServoR.write(intake_setpoint);
    outtakeMotor.set(1);
  }
  else if (PestoLink.buttonHeld(0)) { // Button
    elevatorServoR.write(l1_setpoint);

  }
  else if (PestoLink.buttonHeld(1)) { // Button
    elevatorServoR.write(l2_setpoint);
  }
  else if (PestoLink.buttonHeld(2)) { // Button
    elevatorServoR.write(l3_setpoint);
  }
  else if (PestoLink.buttonHeld(3)) { // Button
    elevatorServoR.write(l4_setpoint);
  }
  //stop intake
  else if (PestoLink.buttonHeld(8)) { // Rebind
    outtakeMotor.set(0)
  }
  //outtake coral
  else if (PestoLink.buttonHeld(9)) { // Rebind
    outtakeMotor.set(1);
  }

  PestoLink.update();

}
