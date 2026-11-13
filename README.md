  # Name: Jonathan Ortiz Guzman
  # Project: Solar Powered Automatic Fan v4
  
  ## Description:
  A fan that is controlled by an Arduino Nano which takes input from a temperature sensor (LM35dt) and controls a MOSFET to turn the fan off or on. 
  
  The whole system is powered by a 12V battery that is charged by a 100W solar panel. To power the logic circuitry, a buck converter lowers the voltage to 5V.

  ## Hardware Variants
  - Arduino Nano: current physical build
  - Pico 2W: wireless development branch
  - ESP32: alternative wireless development branch

  ## Other Repository Branches
  - nano-history - reconstructed Nano code history used to preserve earlier project versions
  - wireless - original wireless development history preserved for reference

  ## Documentation
  - docs/images/ contains current physical setup photos
  - Block diagram PDF
  - Labeled block diagram PDF
  - Electrical diagram PDF
  - Editable draw.io source of block diagram

  ## Future Development
  - Continue Arduino Nano documentation on main
  - Migrate current physical setup to the Pico 2W
  - Use and solder a PCB for logic-level circuit
  - Keep Pico 2W updates solely on pico2w branch
  - Keep ESP32 updates solely on esp32 branch

  # Notes:
  - Project version updated to reflect revisions of code
  - Added clear variable description for easier debugging
  - Organized code chronologically of checks for fan status
  - Added fixed value to V_REF through consistent voltage testing
  - Added LED lights to identify charge(OFF) and standby(ON) phase
  - Added an ON and OFF cycle to conserve battery charge, consulted with AI for optimal time durations
