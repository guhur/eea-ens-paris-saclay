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

