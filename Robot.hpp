
#include <Greenhouse.h>

class Robot : public Image {
 private:
  int motor_left { 0 }, motor_right { 0 };
  typedef std::function<void()> Task;
  struct TaskWait {
    float64 wait_time;
    Task task;
  };
  std::vector<TaskWait> tasks;
  size_t current { 0 };
  float64 last_time { 0.0 };

  void AddTask (float64 time, Task task) {
    tasks.push_back(TaskWait { time, task });
  }

  void SetMotorsInternal (int left, int right) {
    motor_left = left;
    motor_right = right;
  }

 public:
  Robot ()  :  Image { "images/robot.png" } {
    SetSize (0.25 * Size ());
    ZeroTime ();
    SlapOnFeld ();
  }

  void Travail () {
    const float64 dt = CurTime () - last_time;
    auto trans = dt * Up () * (motor_left + motor_right);
    auto rot = dt * (motor_right - motor_left) * PI / 100.0;

    IncTranslation (trans);
    IncRotation (Vect (0, 0, 1), rot);

    if (current < tasks . size ()) {
      TaskWait& tw = tasks[current];
      if (CurTime () > tw . wait_time) {
        tw . task ();
        ZeroTime ();
        ++current;
      }
      if (current >= tasks . size ()) {
        SetMotorsInternal (0, 0);
      }
    } 
     
    last_time = CurTime ();
  }

  void SetMotors (int l, int r) {
    AddTask (0.01, [this, l, r]() { this -> SetMotorsInternal (l, r); });
  }

  void Wait (float64 seconds) {
    AddTask (seconds, []() {});
  }

  int LeftMotor () const { return motor_left; }
  int RightMotor () const { return motor_right; }
};

inline void InitBackground () {
  auto bg = new Image("images/bg.png");
  bg -> SlapOnFeld ();
  bg -> SetSize(4.0 * bg -> Size ());

  HideNeedlePoints ();
}


