void initialize_wifi() {
    Serial.println("... wait for ESP8266");
    // wait that ESP8266 wakes up
    while (!Serial1);

    Serial.println("... set MUX at 1");
    String buff = "";
    do {
      send_msg("AT+CIPMUX=1");
      String buff = read_blocking();
    } while(buff.indexOf("OK") >= 0);

    Serial.println("... launch TCP server on ESP8266");
    do {
      send_msg("AT+CIPSERVER=1,1336");
      String buff = read_blocking();
    } while(buff.indexOf("OK") >= 0);
   
}

String read_nonblocking(){
  char msg[WIFI_LENGTH] = {0};
  for (int i =0;i<WIFI_LENGTH;i++)
        msg[i]= 0;
  Serial1.readBytes(msg, WIFI_LENGTH);

  for (int i =0;i<WIFI_LENGTH;i++) 
      Serial.print(msg[i]);
  return String(msg);
}

String read_blocking(){
  while(Serial1.available() <= 0);
  return read_nonblocking();
}

void send_msg(char* msg) {
    Serial.print( msg);
    Serial.print("\r\n");
    Serial1.print(msg);
    Serial1.print("\r\n");
}

// non blocking transfer from each serial to the other one
void transferSerials() {
  char wifi_buff[WIFI_LENGTH] = {0};   // for incoming serial data
  if (Serial1.available() > 0) {
        for (int i =0;i<WIFI_LENGTH;i++)
          wifi_buff[i]= 0;
        Serial1.readBytes(wifi_buff, WIFI_LENGTH);

        for (int i =0;i<WIFI_LENGTH;i++) {
          Serial.print(wifi_buff[i]);
        }
  } 
   if (Serial.available() > 0) {
        for (int i =0;i<WIFI_LENGTH;i++)
          wifi_buff[i]= 0;
          
        // read the incoming byte:
        Serial.readBytes(wifi_buff, WIFI_LENGTH);
        
        // say what you got:
        Serial.print("Sending: ");
        for (int i =0;i<WIFI_LENGTH;i++) {
          Serial1.print(wifi_buff[i]);
          Serial.print(wifi_buff[i]);
        }
        Serial.println("");
  } 
}

