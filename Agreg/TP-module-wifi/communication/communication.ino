/******************************************
  PURPOSE:  Read and write 
  Created by Pierre-Louis Guhur 
  DATE:   5/2017

  Materials:
  - Arduino Leonardo (Serial and Serial1)
  - ESP8266

  Pins connection:
  - ESP8266 1 - RX, 2 - TX, 3.3V, GND
  
*******************************************/

#define WIFI_LENGTH 256              // length of a message (in bytes)
#define WIFI_BAUDS  115200           // data rate with serial port

void setup() {
    Serial.begin(WIFI_BAUDS);  // Monitor
    Serial1.begin(WIFI_BAUDS); // connected to ESP8266 
    while (!Serial) ;
    while (!Serial1);
}



void loop() {
    transferSerials();
}



