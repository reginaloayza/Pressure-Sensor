/* Finite State Machine */

class FSM {
public:
  FSM(float initial_value, float threshold) : px(initial_value), threshold(threshold), state(false) { }

  bool operator()(const float x)
  {
    if (x < threshold && px < threshold)
    {
      state = false;
    }
    else if (x < threshold && px >= threshold)
    {
      state = true; // Cycle Completed
    }
    else if (x >= threshold && px < threshold)
    {
      state = false;
    }
    else if (x >= threshold && px >= threshold)
    {
      state = false;
    }

    px = x;
    return state;
  }

  bool get_state() {
    return state;
  }
  
private:
  float px; // previous x
  float threshold;
  bool state;
};
