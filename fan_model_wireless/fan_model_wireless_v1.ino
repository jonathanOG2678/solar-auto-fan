/*
* This code is specifically for a 30pin ESP32 Dev Board
*/
#include <WiFi.h>

enum Phase {
  PHASE_OFF = 0,
  PHASE_ON = 1
};

enum FanState{
  FAN_OFF = 0,
  FAN_ON = 1
};

bool automaticMode = true;
bool manualFanState = false;
unsigned long int phase_start;
Phase current_phase;
FanState fan_state;

const unsigned long int MAX_TIME_ON = 1000UL * 60UL * 1UL; //1 min
const unsigned long int MAX_TIME_OFF = 1000UL * 60UL * 2UL; //2 min

const float ON_TEMP = 30.0; //Celsius
const float OFF_TEMP = 26.0; //Celsius 

//PINS for 30 pin ESP32
const int GREEN_LIGHT_PIN = 18;
const int RED_LIGHT_PIN = 19;
const int FAN_PIN = 23;
const int TEMP_PIN = 34;


// WIFI Configuration
const char* AP_SSID = "";
const char* AP_PASSWORD = "";
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// AI GENERATED HTML CSS CODE
//Webpage Configuration
WiFiServer server(80);
String buildWebpage() {
    String page;

    page += "<!DOCTYPE html>";
    page += "<html lang=\"en\">";
    page += "<head>";
    page += "<meta charset=\"UTF-8\">";
    page += "<meta name=\"viewport\" ";
    page += "content=\"width=device-width, initial-scale=1.0\">";
    page += "<title>Chicken Fan Control</title>";

    page += "<style>";
    page += "body{";
    page += "font-family:Arial,sans-serif;";
    page += "text-align:center;";
    page += "background:#f2f2f2;";
    page += "margin:0;";
    page += "padding:40px 15px;";
    page += "}";

    page += ".panel{";
    page += "display:inline-block;";
    page += "min-width:300px;";
    page += "padding:30px;";
    page += "background:white;";
    page += "border-radius:12px;";
    page += "box-shadow:0 3px 12px rgba(0,0,0,.2);";
    page += "}";

    page += ".reading{";
    page += "font-size:22px;";
    page += "margin:18px;";
    page += "}";

    page += ".value{";
    page += "font-weight:bold;";
    page += "}";

    page += ".high{color:#168a38;}";
    page += ".low{color:#c12d2d;}";

    page += "button{";
    page += "width:150px;";
    page += "padding:15px;";
    page += "margin:12px;";
    page += "border:0;";
    page += "border-radius:8px;";
    page += "font-size:18px;";
    page += "background:#3267d6;";
    page += "color:white;";
    page += "cursor:pointer;";
    page += "}";

    page += "button:hover{opacity:.85;}";

    page += "</style>";
    page += "</head>";
    page += "<body>";
    page += "<div class=\"panel\">";
    page += "<h1>Chicken Fan Control</h1>";

    //Fan Mode Status
    page += "<div class=\"reading\">";
    page += "Mode: ";
    page += "<span id=\"modeState\" class=\"value\">Loading...</span>";
    page += "</div>";

    //Fan Operating Status
    page += "<div class=\"reading\">";
    page += "Fan: ";
    page += "<span id=\"fanState\" class=\"value\">Loading...</span>";
    page += "</div>";

    //Fan Phase Indicator
    page += "<div class=\"reading\">";
    page += "Phase: ";
    page += "<span id=\"phaseState\" class=\"value\">Loading...</span>";
    page += "</div>";    

    //Temperature
    page += "<div class=\"reading\">";
    page += "Temperature: ";
    page += "<span id=\"temperature\" class=\"value\">Loading...</span> &deg;F";
    page += "</div>";    

    //Buttons
    page += "<button onclick=\"setAutoMode()\">";
    page += "Automatic";
    page += "</button>";

    page += "<button onclick=\"manualFanOn()\">";
    page += "Manual ON";
    page += "</button>";

    page += "<button onclick=\"manualFanOff()\">";
    page += "Manual OFF";
    page += "</button>";

    page += "<p id=\"connectionStatus\">Connecting...</p>";
    page += "</div>";
    page += "<script>";

    // Request the latest readings from the Pico.
    page += "async function updateReadings(){";
    page += "try{";
    page += "const response=await fetch('/status',{cache:'no-store'});";
    page += "if(!response.ok){";
    page += "throw new Error('HTTP '+response.status);";
    page += "}";
    page += "const data=await response.json();";

    page += "document.getElementById('modeState')";
    page += ".textContent=data.mode;";

    page += "document.getElementById('fanState')";
    page += ".textContent=data.fanState;";

    page += "document.getElementById('phaseState')";
    page += ".textContent=data.phaseState;";

    page += "document.getElementById('temperature')";
    page += ".textContent=Number(data.temperature).toFixed(1);";

    page += "document.getElementById('connectionStatus')";
    page += ".textContent='Live';";

    page += "}catch(error){";
    page += "document.getElementById('connectionStatus')";
    page += ".textContent='Connection lost';";
    page += "console.log(error);";
    page += "}";
    page += "}";

    page += "async function sendCommand(path){";
    page += "await fetch(path,{cache:'no-store'});";
    page += "updateReadings();";
    page += "}";

    page += "function setAutoMode(){";
    page += "sendCommand('/auto');";
    page += "}";

    page += "function manualFanOn(){";
    page += "sendCommand('/manual/on');";
    page += "}";

    page += "function manualFanOff(){";
    page += "sendCommand('/manual/off');";
    page += "}";

    // Update immediately when the page opens
    page += "updateReadings();";
    // Then update every 2000 milliseconds (2sec)
    page += "setInterval(updateReadings,2000);";
    page += "</script>";
    page += "</body>";
    page += "</html>";

    return page;
}

String buildStatusJson() {
    const bool fanIsOn = digitalRead(FAN_PIN) == HIGH;
    const float tempC = tempRead();
    const float tempF = (tempC * 1.8) + 32;

    String json;

    json += "{";

    json += "\"mode\":\"";
    json += automaticMode ? "AUTO" : "MANUAL";
    json += "\",";

    json += "\"fanState\":\"";
    json += fanIsOn ? "ON" : "OFF";
    json += "\",";

    json += "\"phaseState\":\"";
    json += current_phase ? "FAN READY" : "CHARGING";
    json += "\",";

    json += "\"temperature\":";
    json += String(tempF, 3);

    json += "}";

    return json;
}

//Updated from Access Point to STA for having Access Point and connection to router
void beginBoardConnect() {
    Serial.println("Starting Router and AP connections...");
    WiFi.mode(WIFI_AP_STA);

    bool APstatus = WiFi.softAP(AP_SSID, AP_PASSWORD); //local backup network
    if (!APstatus) {
        Serial.println("Failed to start AP");
        return;
    }
    Serial.println("Access point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setAutoReconnect(true);

    Serial.println("Attempting router connection...");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
        Serial.print("IP Adddress: ");
        Serial.println(WiFi.localIP());
    }
}

void sendResponse(WiFiClient& client, const String& response, const String& contentType) {
    client.println("HTTP/1.1 200 OK");
    client.print("Content-Type: ");
    client.println(contentType);
    client.println("Cache-Control: no-store");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(response.length());
    client.println();
    client.print(response);    
}

void handleWebClient() {
    WiFiClient client = server.accept();

    if (!client) {
        return;
    }

    String requestLine;
    unsigned long webStartTime = millis();

    while (client.connected() && millis() - webStartTime < 2000) {
        if (!client.available()) {
            continue;
        }

        String line = client.readStringUntil('\n');

        if (requestLine.length() == 0) {
            requestLine = line;
        }

        if (line == "\r") {
            break;
        }
    }

    Serial.println(requestLine);

    // Toggle Fan State Automatically
    if (requestLine.indexOf("GET /auto ") >= 0) {
        automaticMode = true;
        phase_start = millis();
        sendResponse(client, "{\"success\":true,\"mode\":\"AUTO\"}", "application/json");
    }
    // Returns with manual on
    else if (requestLine.indexOf("GET /manual/on") >= 0) {
        automaticMode = false;
        manualFanState = true;
        current_phase = PHASE_ON;
        fan_state = FAN_ON;
        sendResponse(client, "{\"success\":true,\"mode\":\"MANUAL\"}", "application/json");
    }
    // Returns with manual off
    else if (requestLine.indexOf("GET /manual/off") >= 0) {
        automaticMode = false;
        manualFanState = false;
        current_phase = PHASE_OFF;
        fan_state = FAN_OFF;
        sendResponse(client, "{\"success\":true,\"mode\":\"MANUAL\"}", "application/json");
    }

    // Return live readings
    else if (requestLine.indexOf("GET /status ") >= 0) {
        sendResponse(client, buildStatusJson(), "application/json");
    }
    // Return the main webpage.
    else {
        sendResponse(client, buildWebpage(), "text/html");
    }

    delay(1);
    client.stop();
}

float tempRead() {
    //Reads temperature sensor and converts raw mV to degree Celsius (10mV/C)
    int rawMilliVolts = analogReadMilliVolts(TEMP_PIN);
    float tempC = rawMilliVolts / 10.0;
    return tempC;
}

void fanControlLogic() {
    //Below is fan controller logic
    if (automaticMode) {
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

        float tempC = tempRead(); // Gets temperature

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
    else { //Manual Logic
        digitalWrite(FAN_PIN, manualFanState ? HIGH : LOW);
        if (manualFanState) {
            digitalWrite(GREEN_LIGHT_PIN, HIGH);
            digitalWrite(RED_LIGHT_PIN, LOW);
        } else {
            digitalWrite(GREEN_LIGHT_PIN, LOW);
            digitalWrite(RED_LIGHT_PIN, HIGH);
        }
    }
}


void setup() {
    Serial.begin(115200);
    phase_start = millis(); //gets current time directly instead of a variable
    current_phase = PHASE_ON;
    fan_state = FAN_OFF; //Started off as a safety feature
    analogReadResolution(12); //ESP32 30 pin ADC resoulution

    pinMode(GREEN_LIGHT_PIN, OUTPUT);
    pinMode(RED_LIGHT_PIN, OUTPUT);
    pinMode(TEMP_PIN, INPUT);
    pinMode(FAN_PIN, OUTPUT);

    digitalWrite(FAN_PIN, LOW);
    digitalWrite(GREEN_LIGHT_PIN, LOW);
    digitalWrite(RED_LIGHT_PIN, HIGH);

    beginBoardConnect();
    server.begin();
    Serial.println("Web server started");
}

void loop() {
    handleWebClient();
    fanControlLogic();
}