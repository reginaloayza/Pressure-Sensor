#include "Wire.h" //allows communication over i2c devices
#include "sma_filter.h"

const float PRESSURE_THRESHOLD = 10; // in psi

const int pressureInput = A0; //select the analog input pin for the pressure transducer
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds
float pressureValue = 0; //variable to store the value coming from the pressure transducer

int cycle_count = 0;

float get_pressure()
{
  pressureValue = analogRead(pressureInput); //reads value from input pin and assigns to variable
  pressureValue = ((pressureValue-pressureZero)*pressuretransducermaxPSI)/(pressureMax-pressureZero); //conversion equation to convert analog reading to psi
  return pressureValue;
}

SMA_Filter pressure_sma(5); // The argument is the lookback_period
const int LOW_PRESSURE = 1;
const int HIGH_PRESSURE = 0;
int state;

void send_message_to_raspberry_pi(const int n)
{
  Serial.print("Cycle completed: ");
  Serial.println(n);
}

void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
  state = LOW_PRESSURE;
}

void loop() //loop routine runs over and over again forever
{
  float raw_pressure = get_pressure();
  float filtered_pressure = pressure_sma(raw_pressure);

  if (state == LOW_PRESSURE && filtered_pressure > PRESSURE_THRESHOLD)
    state = HIGH_PRESSURE;
  
  if (state == HIGH_PRESSURE && filtered_pressure < PRESSURE_THRESHOLD)
  {
    state = LOW_PRESSURE;
    cycle_count++;
    send_message_to_raspberry_pi(cycle_count);
  }
  
  //Serial.print(filtered_pressure, 1); //prints value from previous line to serial
  //Serial.println("psi"); //prints label to serial
  delay(sensorreadDelay); //delay in milliseconds between read values
}
