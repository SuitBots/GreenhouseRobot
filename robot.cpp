#include "Robot.hpp"

void Setup() {
  InitBackground ();
  auto robot = new Robot ();

  robot -> SetMotors (10, 10);
  robot -> Wait (1.5);
  robot -> SetMotors (-20, 20);
  robot -> Wait (2.51);
  robot -> SetMotors (10, 10);
  robot -> Wait (3.0);
  robot -> SetMotors (-10, -10);
  robot -> Wait (1.5);
  robot -> SetMotors (20, -20);
  robot -> Wait (2.51);
}
