#include <PestoLink-Receive.h>
#include <Alfredo_NoU2.h>
#include <thread>
#include <chrono>

//declare variables for servo pulse width max and min
float servo_min = 1000;
float servo_max = 2000;

//declare variable for outtake delay
float outtake_delay = 1.5;

//declare motors
NoU_Servo elevatorServoR(4, servo_min, servo_max);
NoU_Motor outtakeMotor(6);
//NoU_Servo elevatorServoL(3, servo_min, servo_max);

void setup() {
  // put your setup code here, to run once:

  //start PestoLink connection
  PestoLink.begin("Robonots!");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //declare variables for elevator setpoints
  float l1_setpoint = 10;
  float l2_setpoint = 20;
  float l3_setpoint = 30; 
  float l4_setpoint = 40;
  float intake_setpoint = 25;

  //logic to drive elevator servo to setpoint angle
  //intaking coral
  if (PestoLink.buttonHeld(0)) {
    elevatorServoR.write(intake_setpoint);
    //elevatorServoL.write(intake_setpoint);
    outtakeMotor.set(1);
    //std::this_thread::sleep_for(std::chrono::seconds(outtake_delay)); //creates delay of outtake_delay seconds
    //outtakeMotor.setState(BRAKE);
  }
  //stop intake
  else if (PestoLink.buttonHeld(1)) {
    outtakeMotor.setState(BRAKE);
  }
  //outtake coral
  else if (PestoLink.buttonHeld(2)) {
    outtakeMotor.set(1);
  }
  else if (PestoLink.buttonHeld(15)) {
    elevatorServoR.write(l1_setpoint);
    //elevatorServoL.write(l1_setpoint);
  }
  else if (PestoLink.buttonHeld(13)) {
    elevatorServoR.write(l2_setpoint);
    //elevatorServoL.write(l2_setpoint);
  }
  else if (PestoLink.buttonHeld(14)) {
    elevatorServoR.write(l3_setpoint);
    //elevatorServoL.write(l3_setpoint);
  }
  else if (PestoLink.buttonHeld(12)) {
    elevatorServoR.write(l4_setpoint);
    //elevatorServoL.write(l4_setpoint);
  }
  else if (PestoLink.buttonHeld(5)) {
    outtakeMotor.set(1);
  }

  //update connection
  PestoLink.update();

}
