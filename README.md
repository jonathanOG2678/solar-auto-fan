  # Name: Jonathan Ortiz Guzman
  # Project: Solar Powered Automatic Fan v4
  
  Description: A fan that is controlled by an Arduino Nano which takes input from a temperature sensor (LM35dt)
    and controls a mosfet to turn the fan off or on. The whole system is powered by a 12v battery that is charged
    by a 100W solar panel. To power the logical circuitry, a buck converter lowers the voltage to 5v.

  # Notes:
  - Project version updated to reflect revisions of code
  - Added clear variable description for easier debugging
  - Organized code chronologically of checks for fan status
  - Added fixed value to V_REF through consistent voltage testing
  - Added LED lights to identify charge(OFF) and standby(ON) phase
  - Added an ON and OFF cycle to conserve battery charge, consulted with AI for optimal time durations
