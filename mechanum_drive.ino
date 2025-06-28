#include <PestoLink-Receive.h>
#include <Alfredo_NoU2.h>
#include <cmath>

//declare motors
NoU_Motor frontRightDrive (2);
NoU_Motor backRightDrive (1);
NoU_Motor frontLeftDrive (3);
NoU_Motor backLeftDrive (4);

void setup() {
  // put your setup code here, to run once:

  //start connection throught PestoLink
  PestoLink.begin("Robonots!");

}

void loop() {
  // put your main code here, to run repeatedly:

  //declare variables
  float joystickL_x = 0;
  float joystickL_y = 0;
  float rotation = 0;
  float angle = 0;
  float power = 0;
  float drivetrain_x = 0;
  float drivetrain_y = 0;
  float front_right = 0;
  float back_left = 0;
  float front_left = 0;
  float back_right = 0;
  float maximum = 0;

  //update variables with joystick input
  joystickL_x = 1 * PestoLink.getAxis(0);
  joystickL_y = 1 * PestoLink.getAxis(1);
  rotation = -1 * PestoLink.getAxis(2);

  //run calculations to determine angle + power the drivetrain should move @
  angle = atan2(joystickL_x, joystickL_y);
  power = hypot(joystickL_x, joystickL_y);

  //calculate x and y components of desired drivetrain trajectory
  drivetrain_x = cos(angle - PI / 4);
  drivetrain_y = sin(angle - PI / 4);
  maximum = max(abs(drivetrain_x), abs(drivetrain_y));

  //calculate motor inputs
  front_right = power * drivetrain_y / maximum - rotation;
  back_left = power * drivetrain_y / maximum + rotation;
  front_left = power * drivetrain_x / maximum + rotation;
  back_right = power * drivetrain_x / maximum - rotation;

  //scale down power inputs to acceptable levels (range of -1 ... 1)
  if ((power + abs(rotation)) > 1) {
    front_right /= power + rotation;
    back_left /= power + rotation;
    front_left /= power + rotation;
    back_right /= power + rotation;
  }

  //set power for each motor
  frontRightDrive.set(front_right);
  backLeftDrive.set(back_left);
  frontLeftDrive.set(front_left);
  backRightDrive.set(back_right);

  //update connection
  PestoLink.update();

  //



}
