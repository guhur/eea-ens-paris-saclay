/******************************************
  PURPOSE:  Running a stepper motor from nweb
  Created by Pierre-Louis Guhur after sketchs by Sam Leong and Rudy Schlaf
  DATE:   5/2017

  Materials:
  - Arduino Leonardo (Serial and Serial1)
  - Stepper motor 28BYJ-48
  - ESP8266

  Pins connection:
  - stepper 8, 9, 10, 11, 5V, GND
  - ESP8266 1, 2, 3.3V, GND
  
*******************************************/

#define STEPPER1  8//these are the Arduino pins that we use to activate coils 1-4 of the stepper motor
#define STEPPER2  9
#define STEPPER3  10
#define STEPPER4  11
#define STEPPERDELAY 8 //delay time in ms (min 8 w/o missing steps)
#define WIFI_LENGTH 256              // length of a message (in bytes)
#define WIFI_BAUDS  115200           // data rate with serial port

bool sens = true; // true = forward, false = backward
bool on = false;

void setup() {
    Serial.begin(WIFI_BAUDS);  // Monitor
    Serial1.begin(WIFI_BAUDS); // connected to ESP8266 
    while (!Serial) ;
    
    Serial.println("Initialize stepper");
    initialize_stepper();

    Serial.println("Initialize wifi");
    initialize_wifi();

    Serial.println("Ready!");
}



void loop() {
    String msg = read_nonblocking().substring(11);
    if (msg != "") {
      if(msg.substring(0,6) == "avance") {
          sens = true;
          on = true;
      } else if(msg.substring(0,6) == "recule") {
          sens = false;
          on = true;
      } else if(msg.substring(0,4) == "stop") {
          on = false;
      } 
    }

    if (on && sens) 
        forward();
    else if(on && !sens)
        backward();
    else if(!on)
        step_OFF();
}



