  # Name: Jonathan Ortiz Guzman
  # Project: Solar Powered Automatic Fan v1
  
  Description: A fan that is controlled by an Arduino Nano which takes input from a temperature sensor (LM35dt)
    and controls a mosfet to turn the fan off or on. The whole system is powered by a 12v battery that is charged
    by a 100W solar panel. To power the logical circuitry, a buck converter lowers the voltage to 5v.

  # Notes:
  - A delay has been added to prevent Serial Monitor from being overflowed with print() statements
  - Used AI to generate ReadVcc() to get a more accurate reading for analog output of temperature sensor in Nano.

