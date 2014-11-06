#include "Robot.hpp"

bool CanSeeBeacon (Robot *robot) { return -1 < robot->IRPower (); }

void MainTask (Robot *robot) {
    while (!robot->HasQuit ()) {
        if (CanSeeBeacon (robot)) {
            float ir_dir = robot->IRDirection ();
            float ir_power = robot->IRPower ();

            if (ir_power > 240) { // close enough
                robot->SetMotors (0, 0);
                break;
            } else {
                float speed = 20.0;
                float left = speed / 2.0;  // ??
                float right = speed / 2.0; //?

                robot->SetMotors (left, right);
            }

            INFORM ("The IR Beacon is at location " + ToStr (ir_dir) +
                    " and power " + ToStr (ir_power));
        } else {
            INFORM ("Can not see beacon.");
            break;
        }
        robot->Wait (0.1);
    }
}
