
#define DARWINPOD Serial
#define WIFIMODULE Serial1
#define LENGTH 1000

String nameSSID = "DarwinPod";
String pwd = "111111";

char buff[LENGTH] = {0};

void setup() {
    //Initialisation Serial Ports
    DARWINPOD.begin(115200);
    WIFIMODULE.begin(115200);
    DARWINPOD.setTimeout(500);
    WIFIMODULE.setTimeout(500);
    
    //Test Wifi Module
    //WIFIMODULE.println("AT+RST");
    WIFIMODULE.println("AT");
    delay(1000);
    
    //Set Wifi Access Point (my IP is 192.168.4.1)
    WIFIMODULE.println("AT+CWMODE=2");
    delay(1000);
    
    WIFIMODULE.print("AT+CWSAP=");
    WIFIMODULE.print('"');
    WIFIMODULE.print(nameSSID);
    WIFIMODULE.print('"');
    WIFIMODULE.print(',');
    WIFIMODULE.print('"');
    WIFIMODULE.print(pwd);
    WIFIMODULE.print('"');
    WIFIMODULE.println(",3,0");
    delay(2000);
    
    //Set TCP Server
    WIFIMODULE.println("AT+CIPMUX=1");
    delay(1000);
    
    WIFIMODULE.println("AT+CIFSR");
    delay(1000);
    
    WIFIMODULE.println("AT+CIPSERVER=1,1336");
    delay(1000);

    
    WIFIMODULE.readBytes(buff, LENGTH);
    for (int i=0; i<LENGTH-1; i++) {
      DARWINPOD.print(buff[i]);
    }
    DARWINPOD.println(buff[LENGTH]);
}

void loop() {
  communication();
}

void communication() {
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
                Serial.readBytes(buff, LENGTH);
                for (int i =0;i<LENGTH;i++) {
                  Serial1.print(buff[i]);
                }
                Serial.println("");
        } 
}

