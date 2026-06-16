enum Phase {
  PHASE_OFF = 0,
  PHASE_ON = 1
};

enum FanState{
  FAN_ON = 1,
  FAN_OFF = 0
};

unsigned long int phase_start;
Phase current_phase;
FanState fan_state;
bool debug = false;

const unsigned long int MAX_TIME_ON = 1000UL * 60UL * 30UL; //30 min
const unsigned long int MAX_TIME_OFF = 1000UL * 60UL * 60UL; //60 min

const float V_REF = 5.19; //Nano supply V
const float ON_TEMP = 30.0; //Celsius, 86.0F
const float OFF_TEMP = 26.0; //Celsius 
//PINS
const int GREEN_LIGHT_PIN = 3; //D3 output
const int RED_LIGHT_PIN = 2; //D2 output
const int FAN_PIN = 9; //D9 output
const int TEMP_PIN = A0; //A0 input
//--------------------MAIN------------------------//
void setup() {
  Serial.begin(9600);
  phase_start = millis(); //gets current time directly instead of a variable
  current_phase = PHASE_ON;
  fan_state = FAN_OFF; //Started off as a safety feature
  pinMode(GREEN_LIGHT_PIN, OUTPUT);
  pinMode(RED_LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  digitalWrite(FAN_PIN, LOW);
  digitalWrite(GREEN_LIGHT_PIN, HIGH);
  digitalWrite(RED_LIGHT_PIN, LOW);
}

void loop() {
  // DEBUG: force fan on
  if (debug) {
    digitalWrite(FAN_PIN, HIGH);
    return;
  }

  unsigned long int current_time = millis();
  unsigned long int time_elapsed = current_time - phase_start;

  // Handle current_phase by calculating if elapsed time is over the MAX_TIME
  if (time_elapsed >= MAX_TIME_ON && current_phase == PHASE_ON) {//If ON phase and we go over ON MAX TIME, switch to OFF
    phase_start = current_time;
    current_phase = PHASE_OFF;
  }
  else if (time_elapsed >= MAX_TIME_OFF && current_phase == PHASE_OFF) {//If OFF phase and we go over OFF MAX TIME, switch to ON
    phase_start = current_time;
    current_phase = PHASE_ON;
  }

  //Reads temperature sensor and converts raw mV to degree Celsius (10mV/C)
  int rawReading = analogRead(TEMP_PIN);
  float voltage = rawReading * V_REF / 1023.0;
  float tempC = voltage * 100.0;

  //Handles fan_state based on current state and turns fan on when above ON_TEMP
  if (current_phase == PHASE_OFF) {
    fan_state = FAN_OFF;
  } 
  else {
    if (fan_state == FAN_OFF && tempC >= ON_TEMP) {
      fan_state = FAN_ON;
    }
    else if (fan_state == FAN_ON && tempC <= OFF_TEMP) {
      fan_state = FAN_OFF;
    }
  }
  
  //Handles visual represention of current_phase with LEDs
  if (current_phase == PHASE_ON) {
    digitalWrite(GREEN_LIGHT_PIN, HIGH);
    digitalWrite(RED_LIGHT_PIN, LOW);
  }
  else {
    digitalWrite(GREEN_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN, HIGH);
  }
  
  //Handles MOSFET GATE
  if (fan_state == FAN_ON) {
    digitalWrite(FAN_PIN, HIGH);
  }
  else {
    digitalWrite(FAN_PIN, LOW);
  }
}
