#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>
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
  float rotation = 0;
  float straight = 0;
  float strafe = 0;
  float front_right = 0;
  float back_left = 0;
  float front_left = 0;
  float back_right = 0;

  //update variables with joystick input
  strafe = 1 * PestoLink.getAxis(0);
  straight = -1 * PestoLink.getAxis(1);
  rotation = 1 * PestoLink.getAxis(3);

  //calculate motor inputs
  front_right = straight - rotation - strafe;
  back_left = straight + rotation - strafe;
  front_left = straight + rotation + strafe;
  back_right = straight - rotation + strafe;

  //scale down power inputs to acceptable levels (range of -1 ... 1)
  if ((abs(straight) + abs(strafe) + abs(rotation)) > 1) {
    front_right /= rotation;
    back_left /= rotation;
    front_left /= rotation;
    back_right /= rotation;
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
