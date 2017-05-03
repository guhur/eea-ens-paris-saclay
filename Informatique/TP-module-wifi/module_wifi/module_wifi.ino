#define LENGTH 256
char buff[LENGTH] = {0};   // for incoming serial data

void setup() {
        Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps
        Serial1.begin(115200);     // opens serial port, sets data rate to 9600 bps

    while (Serial1.available() <= 0) {
      // initialisation d'une connexion
      delay(1000);
      Serial.print("Sending: AT");
      Serial1.print("AT");
    }

//    Serial.print("Sending: AT+CIPMUX=1");
//    Serial1.print("AT+CIPMUX=1");
//    Serial.print("Sending: AT+CIPSERVER=1,1336");
//    Serial1.print("AT+CIPSERVER=1,1336");
}

void loop() {
        if (Serial1.available() > 0) {
                for (int i =0;i<LENGTH;i++)
                  buff[i]= 0;
                Serial1.readBytes(buff, LENGTH);

                for (int i =0;i<LENGTH;i++) {
                  Serial.print(buff[i]);
                }
        } 
         if (Serial.available() > 0) {
                for (int i =0;i<LENGTH;i++)
                  buff[i]= 0;
                  
                // read the incoming byte:
                Serial.readBytes(buff, LENGTH);
                
                // say what you got:
//                Serial.print("Sending: ");
                for (int i =0;i<LENGTH;i++) {
                  Serial1.print(buff[i]);
                  Serial.print(buff[i]);
                }
                Serial.println("");
        } 
}

char* reception(){
  char msg[LENGTH] = {0};
  for (int i =0;i<LENGTH;i++) {
         msg[i]= 0;
          Serial1.readBytes(msg, LENGTH);

          for (int i =0;i<LENGTH;i++) {
            Serial.print(buff[i]);
          }
  }
  return msg;
}

