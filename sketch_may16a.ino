// Constants for voltage calculations
const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)

//Define pins here, time values, booleans
const int fanPin = 9;
const int tempPin = A0;
bool fanOn = false;
const unsigned long ON_TIME_MAX = 1000 * 10; //10 Seconds ON_TIME
const unsigned long OFF_TIME_MAX = 1000 * 20; //20 Seoconds OFF_TIME
const float onTemp = 30.0; //~78.8 Farenheit
const float offTemp = 28.0; //~82.4 Farenheit

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
}

void loop() {
  //1. Read temperature from sensor analogRead(A0)
  int rawTempRead = analogRead(tempPin);
  float voltage = (rawTempRead / ADC_STEPS) * V_REF; //converts to voltage
  float temperatureC = voltage * 100.0;        // LM35: 10mV per °C
  float temperatureF = (temperatureC * (9.0/5.0)) + 32.0;

  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | Temperature: ");
  Serial.print(temperatureC, 3);
  Serial.print(" C, ");
  Serial.print(temperatureF, 3);
  Serial.println(" F");

  //2. Check if temperature is in range AND fanOn
  if (!fanOn && temperatureC >= onTemp){
    digitalWrite(fanPin, HIGH);
    fanOn = true;   
  }
  else if (fanOn && temperatureC <= offTemp){
    digitalWrite(fanPin, LOW);
    fanOn = false;
  }
  delay(1000);
}
