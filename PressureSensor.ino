#include "fsm.h"
#include "sma_filter.h"
#include "pressure_sensor_driver.h"

const float THRESHOLD = 10; // in psi (this is the threshold for the cycle detection)
int cycle_count = 0;
SMA_Filter sma_filter(5); // The argument is the lookback_period
FSM fsm(0, THRESHOLD);

void send_message_to_raspberry_pi(const int n) {
  Serial.print("Cycle completed: ");
  Serial.println(n);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  float raw_signal = get_pressure();
  float filtered_signal = sma_filter(raw_signal);

  if (fsm(filtered_signal)) {
    cycle_count++;
    send_message_to_raspberry_pi(cycle_count);
  }
}
