const int fanPin = 9;      // D9: output to LED/fan
const int tempPin = A0;    // A0: LM35 output
const float onTemp = 30.0;
const float offTemp = 28.0;
bool fanOn = false;

long readVcc() {
  ADMUX = _BV(REFS0)             // Reference = Vcc
        | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // Measure internal 1.1V
  delay(2);                      // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);           // Start ADC conversion
  while (bit_is_set(ADCSRA, ADSC)); // Wait for completion
  uint16_t result = ADC;
  long vcc = 1125300L / result;  // 1125300 = 1.1V * 1023 * 1000
  return vcc; // in millivolts
}

void setup()
{
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
}

void loop()
{
  int reading = analogRead(tempPin);
    long supplyVoltage = readVcc();              // Get Vcc in mV
  float voltage = reading * (supplyVoltage / 1000.0) / 1023.0; // volts
  float temperatureC = voltage * 100.0;        // LM35: 10mV per °C
  float temperatureF = (temperatureC * (9.0/5.0)) + 32.0;
  // Fan control with hysteresis
  if (!fanOn && temperatureC >= onTemp) 
  {
    digitalWrite(fanPin, HIGH);
    fanOn = true;
  } 
  else if (fanOn && temperatureC <= offTemp) 
  {
    digitalWrite(fanPin, LOW);
    fanOn = false;
  }

  // Debugging output
Serial.print("Temp: ");
Serial.print(temperatureF);
Serial.print(" °F | ");
Serial.print(temperatureC);
Serial.println(" °C");


  delay(1000);
}
