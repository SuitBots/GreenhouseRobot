
#include <Greenhouse.h>
#include <unistd.h>
#include <mutex>
#include <future>

static const char *IR_BEACON_NAME = "IrBeacon";

class Robot {
public:
    virtual ~Robot () {}

    virtual bool HasQuit () = 0;

    virtual void SetMotors (int left_motor, int right_motor) = 0;
    virtual void Quit () = 0;

    virtual void Wait (float seconds) { usleep (1000000 * seconds); }
    virtual int IRDirection () { return -1; };
    virtual int IRPower () { return -1; }
    virtual int Compass () { return -1; }
    virtual int USFront () { return -1; }
    virtual int USRear () { return -1; }
    virtual int Gyroscopic () { return -1; }
    virtual int AccelrationX () { return 0; }
    virtual int AccelrationY () { return 0; }
    virtual int LeftMotorEncoder () { return -1; }
    virtual int RightMotorEncoder () { return -1; }
};

class IRBeacon : public Image {
public:
    IRBeacon () : Image{"images/circle.png"} {
        SetName (IR_BEACON_NAME);
        SetSize (25.0);
        SetAdjColor (Color (1.0, 0.0, 0.0, 1.0));
        ColorAnimateSine (Color (1.0, 0.0, 0.0, 0.5), 2.5);
    }
};

class RobotThing : public Image, public Robot {
private:
    bool started{false};
    std::function<void(Robot *)> task;
    std::thread th;
    std::mutex mutex;
    typedef std::lock_guard<std::mutex> lock_t;
    float64 last_time;
    int motor_left{0}, motor_right{0};
    bool quit{false};
    Vect start_loc;

    void Restart () {
        if (started) {
            quit = true;
            th.join ();
        } else {
            started = true;
            start_loc = Loc ();
        }
        this->SetMotors (0, 0);
        this->SetTranslationHard (start_loc);
        this->RotateLike (Feld ());
        quit = false;
        th = std::thread ([this]() { task (this); this -> Quit (); });
    }

    Vect IRSensorLoc () {
        Vect loc = Loc ();
        Vect up = Up ();
        float64 size = Size ();
        return loc + up * size / 2.0;
    }

public:
    RobotThing (std::function<void(Robot *)> task_)
        : Image{"images/robot.png"}, task (task_) {
        SetSize (0.25 * Size ());
        ZeroTime ();
        SlapOnFeld ();
    }

    virtual ~RobotThing () {
        if (started) {
            th.join ();
        }
    }

    virtual void Blurt (BlurtEvent *e) {
        if (Utters (e, "R")) {
            Restart ();
        }
    }

    void Travail () {
        if (!started) {
            Restart ();
        }
        lock_t lock (mutex);
        const float64 dt = CurTime () - last_time;
        auto trans = dt * Up () * (motor_left + motor_right);
        auto rot = dt * (motor_right - motor_left) * PI / 100.0;
        IncTranslation (trans);
        IncRotation (Vect (0, 0, 1), rot);
        last_time = CurTime ();
    }

    // The Robot interface
    virtual void SetMotors (int left, int right) {
        lock_t lock (mutex);
        motor_left = left;
        motor_right = right;
    };
    virtual void Quit () {
        lock_t lock (mutex);
        quit = true;
        motor_left = motor_right = 0;
    };

    virtual bool HasQuit () { return quit; }

    virtual void Wait (float seconds) { usleep (1000000 * seconds); }

    virtual int IRDirection () {
        auto beacon = FindNode (IR_BEACON_NAME)->As<IRBeacon>();
        if (nullptr == beacon) {
            return -1;
        }
        int power = IRPower ();
        auto diff = (beacon->Loc () - IRSensorLoc ());
        auto up = Up (); //.Norm ();

        float64 angle = Deg (diff.AngleWith (up, Norm ()));

        if (angle > 120.0 || angle < -120.0) {
            return -1;
        }
        if (0 == power) {
            return -1;
        }

        return 1 + (int)(angle + 120.0) / 26.66;
    };

    virtual int IRPower () {
        auto beacon = FindNode (IR_BEACON_NAME)->As<IRBeacon>();
        if (nullptr == beacon) {
            return -1;
        }
        auto diff = IRSensorLoc () - beacon->Loc ();
        auto mag = diff.Mag ();

        auto distance = 18.0 * mag / Size ();

        const float64 max_distance = 60.0;

        if (distance > max_distance) {
            return -1;
        }

        auto pct = (max_distance - distance) / max_distance;
        return (int)255 * pct;
    }

    virtual int Compass () { return -1; }
    virtual int USFront () { return -1; }
    virtual int USRear () { return -1; }
    virtual int Gyroscopic () { return -1; }
    virtual int AccelrationX () { return 0; }
    virtual int AccelrationY () { return 0; }
    virtual int LeftMotorEncoder () { return -1; }
    virtual int RightMotorEncoder () { return -1; }
};

inline void InitBackground () {
    auto bg = new Image ("images/bg.png");
    bg->SlapOnFeld ();
    bg->SetSize (4.0 * bg->Size ());

    HideNeedlePoints ();
}

void MainTask (Robot *r);

void Setup () {
    InitBackground ();
    auto robot = new RobotThing (MainTask);
    robot->SlapOnFeld ();

    auto beacon = new IRBeacon ();
    beacon->SlapOnFeld ();

    auto rs = robot->Size ();

    // robot->IncTranslationHard (rs * 3 * robot->Over ());
    beacon->IncTranslationHard (2 * rs * robot->Up () +
                                2 * rs * robot->Over ());
    // beacon->IncTranslationHard (-rs * 3 * robot->Over ());
}
