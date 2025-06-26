#include <Alfredo_NoU2.h>
#include <PestoLink-Receive.h>

  NoU_Motor backLeftDrive(4);
  NoU_Motor frontLeftDrive(3);
  NoU_Motor frontRightDrive(2);
  NoU_Motor backRightDrive(1);
  NoU_Drivetrain drivetrain(&frontLeftDrive, &frontRightDrive, &backLeftDrive, &backRightDrive);


void setup() {
  // put your setup code here, to run once:
  PestoLink.begin("Hello!"); // Change name

  backLeftDrive.set(0);
  frontLeftDrive.set(0);
  frontRightDrive.set(0);
  backRightDrive.set(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  float rotation = 0;
  float throttle = 0;

  rotation = -1 * PestoLink.getAxis(0);
  throttle = 1 * PestoLink.getAxis(3);

  drivetrain.arcadeDrive(throttle, rotation);

  PestoLink.update();

}
