/*****************************************************
  Darwipod
  Equilibre du gyropode
  Algorithme tres simple : verticalité

  23/03/2016 : version de départ d'Alice Delmer (et Cie)
  (inutilisable en l'etat : gyropode tombe immédiatement)

  mod 10/04/2016 PV
  - gestion frottements
  - corrections erreurs d'implémentation
  - simplification et ralentissement des affichages
  - double -> float (pas de vrai double sur uno)
  - passage argument de control() en global

  mod 12/04/2016 PV
  - ajustement des offsets du capteur
  - ajout gestion LCD
  - réglage des offsets du capteur

 *****************************************************/

//#define USE_LCD
//#define TEST_POT_PWM
#define PWM_ENABLE
#define USE_MPU6050

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#ifdef USE_LCD
#include <LiquidCrystal_I2C.h>
#endif

// paramètres de fonctionnement
#define outMax 220         // limitation PWM des moteurs
#define outMin 35
#define ANGLE_LIMITE 45    // au delà on est sûr de tomber
#define ITermMax 150       // limite valeur intégrale
#define frottements 4      // compensation frottements secs
#define LED_PIN 13         // sortie 13 sur pro mini
#define MPU6050_IRQ_PIN 2  // interruption de synchro réception données MPU
#define RX_IRQ_PIN 3       // réception des impulsions 
#define In_kp A0     // potentiomètre action proportionnelle
#define In_ki A1     // potentiomètre pour l'action intégrale
#define In_kd A2     // potentiomètre pour l'action dérivée
#define I2C_ADR1 4   // adresses cartes moteurs
#define I2C_ADR2 6
#define NEUTRE_RC 1500  // valeur en us

// instanciation des objets
#ifdef USE_MPU6050
MPU6050 mpu;
#endif
#ifdef USE_LCD
LiquidCrystal_I2C lcd(0x27, 32, 4); // afficheur 4 lignes 20 caractères
char lcdbuff[21];
#endif


bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int16_t gyro[3];
float inclinaison;
unsigned long timer;
float angleInit;


char disp_counter;  // pour affichage cyclique
float Setpoint;  // angle de référence
int traject;
char serialbuff[80]; // TODO: eliminer si pas utilisé

// ISR pour MPU6050 (données prêtes)
// periode 10 ms environ

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
/*********************************************
  ISR de réception de la télécommande
  décodage des impulsions reçues
  - 3 impulsions -> 5 voies
  - intertrain > 5ms
 **********************************************/
volatile int rx_pulses[5];

void rx_ISR() {
  static int raw_pulses[5];
  static unsigned long last_time;
  static char edge_count;
  unsigned long now;
  unsigned duree;
  char i;
  now = micros();
  duree = now - last_time;
  if (duree > 5000) { // intertrain ?
    if (edge_count == 5) { // bien place ?
      // envoi des valeurs
      for (i = 0; i < 5; rx_pulses[i++] = raw_pulses[i]);
      //     memcpy(rx_pulses,raw_pulses,sizeof(raw_pulses));
    }
    edge_count = 0;
  } else { // duree d'une voie
    if (edge_count > 4) // init automatique
      edge_count = 0;
    else
      raw_pulses[edge_count++] = (int)duree - NEUTRE_RC;
  }
  last_time = now;
}
/*********************************************
   gestion de l'équilibre
   tout est passé en variables globales
 *********************************************/
void control(void) {
  // Ces valeurs doivent être conservées d'un appel à l'autre
  static unsigned long lastTime;
  static float lastErr;
  static float lastInput;
  static float ITerm;
  /*
    // PID constants
    float kp = analogRead(In_kp)/50.;
    float ki = analogRead(In_ki)/250.;
    float kd = analogRead(In_kd)/1000.;
  */
  float kp = 13;
  float ki = 0;  //
  float kd = 0;

#ifdef USE_LCD
  if (disp_counter == 5) {

    lcd.setCursor(0, 2);
    dtostrf(kp, 6, 2, lcdbuff);
    lcd.print(lcdbuff);
    dtostrf(ki, 6, 2, lcdbuff);
    lcd.print(lcdbuff);
    dtostrf(kd, 6, 2, lcdbuff);
    lcd.print(lcdbuff);
  }
#endif
  /*  Serial.print(kp);
  	Serial.print("; ");
  	Serial.print(ki);
  	Serial.print("; ");
  	Serial.println(kd);
  */

  //PID output

  int itimeChange = (int)(timer - lastTime); // période d'échantillonnage : env. 10ms
  //  Serial.print("\t");
  //  Serial.println (itimeChange);
  float timeChange = ((float)itimeChange) * 0.001;
  float error = Setpoint - inclinaison;
  ITerm += (error * timeChange);
  if (abs(ITerm) > ITermMax)
    ITerm = (ITerm > 0) ? ITermMax : -ITermMax;
  float dInput = (error - lastErr) / timeChange;
  int output = 127 + (unsigned)(kp * error + ki * ITerm + kd * dInput + 0.5);



  lastInput = inclinaison;
  lastErr = error;
  lastTime = timer;

  //  Serial.print("S ");
  //  Serial.println((byte)output);
  //  Serial.print("E ");
  //  Serial.println(error);

#ifdef PWM_ENABLE
  if (abs(error) < ANGLE_LIMITE) {
    // traject donne la différence de commande entre les moteurs
    // (rotation du giropode)
    int out1 = output + traject;
    if (out1 > 128)
      out1 += frottements;
    if (output < 127)
      out1 -= frottements;
    if (out1 > outMax)
      out1 = outMax;
    else
      if (out1 < outMin) out1 = outMin;
    int out2 = output - traject;
    if (out2 > 128)
      out2 += frottements;
    if (output < 127)
      out2 -= frottements;
    if (out2 > outMax)
      out2 = outMax;    
    if (out2 > outMax)
      out2 = outMax;
    else
      if (out2 < outMin) out2 = outMin;
    // envoi des valeurs PWM sur les cartes de contrôle des moteurs  
    Wire.beginTransmission(I2C_ADR1);
    Wire.write((byte)(out1));
    Wire.endTransmission();
    Wire.beginTransmission(I2C_ADR2);
    Wire.write((byte)(out2));
    Wire.endTransmission();
  } else {       // si on tombe : arret d'urgence !
    Wire.beginTransmission(I2C_ADR1);
    Wire.write(127);
    Wire.endTransmission();
    Wire.beginTransmission(I2C_ADR2);
    Wire.write(127);
    Wire.endTransmission();
  }
#endif
}
/**************************************************/
void setup() {
#ifdef USE_LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Darwipod");
#else
  Serial.begin(115200);
  Wire.begin(); // (I2Cdev library doesn't do this automatically)
#endif
  // passage en I2C 400 kHz
  //    sercom3.disableWIRE();                         // Disable the I2C bus
  //    SERCOM3->I2CM.BAUD.bit.BAUD = SystemCoreClock / ( 2 * 400000) - 1 ;   // // Set the I2C SCL frequency to 400kHz
  //    sercom3.enableWIRE();                          // Restart the I2C bus
  TWBR = 12; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#ifdef USE_MPU6050  
#ifdef USE_LCD
  lcd.setCursor(0, 3);
  lcd.print("Init MPU");
  delay(1000);
#else
  Serial.println(F("Init MPU"));
#endif
  mpu.initialize();
  // verify connection
#ifdef USE_LCD
  lcd.setCursor(0, 3);
  lcd.print(mpu.testConnection() ? "MPU6050 linked  " : "MPU6050 link HS ");
  delay(1000);
#else
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
#endif
  delay(2000);
#ifdef USE_LCD
  lcd.setCursor(0, 3);
  lcd.print("                ");
#endif
  // load and configure the DMP
  //    Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(105);
  mpu.setYGyroOffset(-26);
  mpu.setZGyroOffset(-28);
  mpu.setXAccelOffset(-2041);
  mpu.setYAccelOffset(318);
  mpu.setZAccelOffset(481);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    //        Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);



    // enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(MPU6050_IRQ_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    //        Serial.print(F("DMP Initialization failed (code "));
    //        Serial.print(devStatus);
    //        Serial.println(F(")"));
  }
#endif  // USE_MPU6050 
  // mise en place ISR telecommande
  attachInterrupt(digitalPinToInterrupt(RX_IRQ_PIN), rx_ISR, CHANGE);
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  pinMode(In_kp, INPUT);
  pinMode(In_ki, INPUT);
  pinMode(In_kd, INPUT);

}
/**************************************************/
void loop() {
  byte i2c_read_buff[5];
  char i;
  unsigned position1, position2;
  int vitesse1, vitesse2;
  while (1) {
    timer = millis();

#ifdef USE_MPU6050
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
      // le code eventuel placé ici doit s'exécuter TRES rapidement
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
#ifdef USE_LCD
      lcd.setCursor(0, 3);
      lcd.print("FIFO overflow!");
#else
      Serial.println(F("FIFO overflow!"));
#endif
      // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {

      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);

      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;

      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGyro(gyro, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      inclinaison = ypr[2] * 180 / M_PI;
      // Affichage une fois sur 10
      disp_counter = (++disp_counter) % 10;
      if (disp_counter == 0) {
#ifdef USE_LCD
        lcd.setCursor(0, 1);
        dtostrf(inclinaison, 6, 2, lcdbuff);
        lcd.print(lcdbuff);
#else
        Serial.println(inclinaison);
#endif
      }
      
      //      if (disp_counter==1) {
      //          sprintf(serialbuff,"%d %d %d %d %d",rx_pulses[0],rx_pulses[1],rx_pulses[2],rx_pulses[3],rx_pulses[4]);
      //          Serial.println(serialbuff);
      //      }
      if (rx_pulses[3] > 0) {
        Setpoint = -(float)rx_pulses[1] / 20;
        traject = round((float)rx_pulses[2] / 30);
      } else {
        Setpoint = 0;
        traject = 0;
      }
    }
    control();
#else
#ifdef TEST_POT_PWM
byte  output = analogRead(In_kp)>>2;
//  Wire.beginTransmission(I2C_ADR1);
//  Wire.write(output);
//  Wire.endTransmission();
  Wire.beginTransmission(I2C_ADR2);
  Wire.write(output);
  Wire.endTransmission();
#else
    delay(50);
    Wire.beginTransmission(I2C_ADR2);
    Wire.write(127);
    Wire.endTransmission();
#endif    
#endif // USE_MPU6050 

    // récupération des infos des moteurs
    i = 0; // init pointeur rx
    Wire.requestFrom(I2C_ADR2, 5);    // 5 octets issus de la carte 1
    while (Wire.available()) { // slave may send less than requested
      i2c_read_buff[i++] = Wire.read(); // receive a byte as character
    }
    vitesse1 = (i2c_read_buff[1] << 8) | i2c_read_buff[0];
    position1 = (i2c_read_buff[3] << 8) | i2c_read_buff[2];
    // TODO : checksum
#ifdef USE_LCD
#ifdef USE_MPU6050    
    if (disp_counter == 8) {
      sprintf(lcdbuff,"%4d",vitesse1)
      lcd.setCursor(6, 1);
      lcd.print(lcdbuff);
    }
    if (disp_counter == 9) {
      sprintf(lcdbuff,"%4d",position1)
      lcd.setCursor(11, 1);
      lcd.print(lcdbuff);
    }
#else // USE_MPU6050 désactivé
    lcd.setCursor(0, 1);
    sprintf(lcdbuff,"%02x %02x %02x %02x %02x",
        i2c_read_buff[0],i2c_read_buff[1],i2c_read_buff[2],i2c_read_buff[3],i2c_read_buff[4]);
    lcd.print(lcdbuff);
    lcd.setCursor(0, 2);
    sprintf(lcdbuff,"V %4d P %4d ",vitesse1,position1);
    lcd.print(lcdbuff);
#endif
#endif
  }
}




