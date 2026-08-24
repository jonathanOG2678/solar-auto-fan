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

  - The IRLZ44N MOSFET that I have is not ideal for a HIGH LOW outputs on an ESP32 as it is a 3.3V logic board. I noticed this behavior in a model that I built of the solar fan system and it started flickering the fan load. After looking at the MOSFET documentation, I noticed it does not mention 3.3V being sufficuent to mimic HIGH or LOW behavior but float as a partial open/close.

 # Future Ideas
  - Since my limitation is connection distance, I was thinking of putting something in between the ESP32 and the router to help relay information. This has been described as a proxy by AI where the devices in between my source and destination devices make requests on behalf of them. I am thinking of having one or two Android phones for this purpose. I need to study how this is possible.
  - Since the ESP32 is a lower voltage device than the Arduino Nano, I would need to change the MOSFET to be able to mimic HIGH or LOW behavior with the lower 3.3V of the board. I also thought of trying to increase the voltage back to 5V in series of the ESP32 output pin but I am not sure how this would affect the behavior of a fully open or closed MOSFET.