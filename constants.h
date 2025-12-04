#ifndef CONSTANTS
#define CONSTANTS

// Timings
constexpr int autoTimeout = 15000;
constexpr int autoDriveTimeout = 10000;
constexpr int autoOuttakeTimeout = autoTimeout - autoDriveTimeout;

// Elevator Positioning
constexpr int l1Setpoint = 0;
constexpr int l2Setpoint = 600;
constexpr int l3Setpoint = 1200;

// Button Bindings
constexpr int l1Button = 0;
constexpr int l2Button = 2;
constexpr int l3Button = 1;
constexpr int outtakeButton = 7;
constexpr int reverseOuttakeButton = 6;
constexpr int toggleElevator = 13;
constexpr int enableAuto = 12;
constexpr int elevatorUpButton = 7;
constexpr int elevatorDownButton = 6;

#endif