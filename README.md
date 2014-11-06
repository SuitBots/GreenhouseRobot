# Greenhouse Robot

You'll need [Oblong's Greenhouse SDK](http://greenhouse.oblong.com/) to run this.

## Building

Assuming Greenhouse is installed per usual, run `make`.

## Use

Edit [robot.cpp](robot.cpp). In the setup function, after the new robot is created, you can
call any of the following functions:

    robot -> Wait (time); // wait for this long
    robot -> SetMotors (left_motor, right_motor); // Set the motors
    robot -> IRPower (); // -1 for not found, 0-255 otherwise
    robot -> IRDirection (); // -1 for not found, 1 - 9 left-to-right otherwise

The robot has a tank drive.
