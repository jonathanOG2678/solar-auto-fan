  # Name: Jonathan Ortiz Guzman
  # Project: Solar Powered Automatic Fan v5
  
  Description: A fan that is controlled by an ESP32 which takes input from a temperature sensor (LM35dt)
    and controls a mosfet to turn the fan off or on. The whole system is powered by a 12v battery that is charged
    by a 100W solar panel. To power the logical circuitry, a buck converter lowers the voltage to 5v.

  # Notes:
  - Project version updated to reflect revisions of code
  - Created a basic webpage using AI that shows the fan status and temperature of the environment 
  - Webpage refreshes the page to show live readings of the system
  - Added a manual and automatic feature that can toggle fan operation
  - Used Access Point and Station mode on the ESP32 to connect to the system
  - Coverted my original fan logic code as a function to simplify main

  # Things I Have Learned:
  - In building the wireless version to see the temperature values and the fan status, I originally used a Raspberry Pi Pico 2W. Here is where I learned the basic functions to wireless programming in the Arduino IDE where I installed the Arduino Pico library to load the program onto memory. The Raspberry Pi Pico 2W limitation was router connection being to far away and I wanted to have it always have a connection. I learned through AI conversations and documentation hunting about the possible modes it can have such as Access Point (AP) and Station (STA) but with the ESP32, it can handle both simultaneously. I am still confused about the webclient checking and sending responses via json to update the HTML and use CSS to interact with the page. 
