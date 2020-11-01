#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(3, 2,false);
//#define mySerial Serial1

// WiFi Config
const char* ssid     = "Ananda";
const char* password = "k99229922";

// FireBase Config
#define FIREBASE_HOST "embeded-project-866b7.firebaseio.com"
#define FIREBASE_AUTH "HjXKfEujpW6a0TKwQrgTI4kKXfBBdOdncIjTrEG7"

FirebaseData firebaseData;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

#define RXD2 16
#define TXD2 17
#define solenoidLogic 32
#define swLock 33
#define swMode 26
#define swPlus 18
#define swMinus 19
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
uint8_t id;
bool IsLock =true;
bool ChStatLock;
unsigned long period = 1000;
unsigned long last_time = 0;
unsigned long last_time2 = 0; 
int count=0;
int countSet=0;
int modee=1; //1=check 2=set
int fgSlot=0;
//hw_timer_t * timer = NULL;

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  pinMode(solenoidLogic, OUTPUT);
  pinMode(swLock, INPUT_PULLUP);
  pinMode(swMode, INPUT_PULLUP);
  pinMode(swPlus,INPUT_PULLUP);
  pinMode(swMinus,INPUT_PULLUP);
  
  Serial.begin(9600);  // Initialize serial communications with the PC
  // Print a message to the LCD.
  int check;
//  timer = timerBegin(0, 40000, true);
//  timerAttachInterrupt(timer, &lock, true);
//  timerAlarmWrite(timer, 10000, true);
//  timerAlarmEnable(timer);

  //Wifi connect
  WiFi.softAPdisconnect (true);
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
    }
  }
  Serial.println("WIFI connected");

  //Connect Database
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase connected");
  
  //attachInterrupt(swLock, lock, FALLING);
  while (!Serial);            // Do nothing if no serial port is opened
  finger.begin(57600);        // Init Fingerprint
  Serial.println("Please Scan ");
     
    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
      
}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void maindsp()
{ 
 
   Serial.println("Please Scan");
 }
void NoFingerdetec()
{   
    Serial.println("No match found");
    delay(1000);
}
void again()
{ 
  Serial.println("Pls. Scan Again");
  delay(1000);
  //lcd.clear();
  return maindsp();
}

void loop() {
      
      if(digitalRead(swMode)==false)
      {
        if( millis() - last_time2 > period) {
        
        last_time2 = millis();
        countSet++;
        }
      }
      if(countSet>=3)
      {
          Serial.println("Ready to enroll a fingerprint!");
          display.clearDisplay();
          display.setTextSize(1); // Draw 2X-scale text
          display.setTextColor(WHITE);
          display.setCursor(30, 15);
          display.println("Select Slot:"+String(fgSlot));
          display.setCursor(50,0);
          display.setTextSize(1);
          display.println("mode: saving");
          display.display();
          Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
          //id = readnumber();
          if(digitalRead(swPlus)==false){
            if(fgSlot<127){
               fgSlot++;
            }
            display.setCursor(30, 15);
            display.println("Select Slot:"+String(fgSlot));
            display.display();
          }
         if(digitalRead(swMinus)==false){
          if(fgSlot>1){
               fgSlot--;
            }
            display.setCursor(30, 15);
            display.println("Select Slot:"+String(fgSlot));
            display.display();
          }
          if(digitalRead(swMode)==false)
          {
            id=fgSlot;
          }
          if (id == 0) {// ID #0 not allowed, try again!
             return;
          }
          Serial.print("Enrolling ID #");
          Serial.println(id);
        
          while (!  getFingerprintEnroll() );
        
        countSet=0;
      }else{
          if(digitalRead(swLock)==false){
            digitalWrite(solenoidLogic, 0);
            Firebase.setBool(firebaseData, "/status", true);
            Firebase.setBool(firebaseData, "/statusApp", true);
            count=0;
            IsLock=true;
            Serial.println("locked");
          }
          Firebase.getBool(firebaseData, "/statusApp");
          ChStatLock=firebaseData.boolData();
          if(firebaseData.boolData())
          {
            IsLock=true;
          }else{
            IsLock=false;
          }
          if(IsLock==true)
        {
          display.clearDisplay();
          display.setTextSize(2); // Draw 2X-scale text
          display.setTextColor(WHITE);
          display.setCursor(0, 8);
          display.println("LOCKED");
          //Firebase.getBool(firebaseData, "/statusApp");
          Firebase.setBool(firebaseData, "/status", true);
          //if(!firebaseData.boolData())
          //Firebase.setBool(firebaseData, "/statusApp", true);
          display.display();
          digitalWrite(solenoidLogic, 0);
        }else{
          display.clearDisplay();
          display.setTextSize(2); // Draw 2X-scale text
          display.setTextColor(WHITE);
          display.setCursor(0, 8);
          display.println("UNLOCKED");
          //Firebase.getBool(firebaseData, "/statusApp");
          Firebase.setBool(firebaseData, "/status", false);
          //if(firebaseData.boolData())
          //Firebase.setBool(firebaseData, "/statusApp", false);
          display.display();
          digitalWrite(solenoidLogic, 1);
        }
        getFingerprintIDez();
        if(IsLock== false){
          if( millis() - last_time > period) {
          
            last_time = millis();
            count++;
  
            if(count>=5)
            {
              IsLock=true;
              Firebase.setBool(firebaseData, "/status", true);
              Firebase.setBool(firebaseData, "/statusApp", true);
              digitalWrite(solenoidLogic, 0);
              count=0;
            } 
          }
        }
            
       }
}
            

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
  NoFingerdetec();
  again();
  return -1;
  }
    //open_lock();
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(solenoidLogic, true);
  Firebase.setInt(firebaseData, "/status", false);
  Firebase.setBool(firebaseData, "/statusApp", false);
  IsLock=false;
  return finger.fingerID; 
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("PLACE FINGER");
  display.setCursor(50,0);
  display.setTextSize(1);
  display.println("mode: saving");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("REMOVE FINGER");
  display.setCursor(50,0);
  display.setTextSize(1);
  display.println("mode: saving");
  display.display();
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(30, 15);
  display.println("PLACE FINGER");
  display.setCursor(50,0);
  display.setTextSize(1);
  display.println("mode: saving");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
