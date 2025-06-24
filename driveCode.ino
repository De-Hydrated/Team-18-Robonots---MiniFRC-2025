#include <Alfredo_NoU2.h>
#include <PestoLink_Recieve.h>

  NoU_Motor backLeftDrive(4);
  NoU_Motor frontLeftDrive(3);
  NoU_motor frontRightDrive(2);
  NoU_Motor backRightDrive(1);
  NoU_Drivetrain drivetrain(&backLeftDrive, &frontLeftDrive, &frontRightDrive, &backRightDrive);


void setup() {
  // put your setup code here, to run once:
  PestoLink.begin("Hello!");
}

void loop() {
  // put your main code here, to run repeatedly:
  float rotation = 0;
  float throttle = 0;

  rotation = -1 * PestoLink.getAxis(0);
  throttle = 1 * PestoLink.getAxis(1);

  drivetrain.arcadeDrive(throttle, rotation);

  PestoLink.update();

}
