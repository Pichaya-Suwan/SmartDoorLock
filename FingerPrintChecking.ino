#include <Adafruit_Fingerprint.h>
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(3, 2,false);
//#define mySerial Serial1
#define RXD2 16
#define TXD2 17
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
void setup() {
  
  Serial.begin(9600);  // Initialize serial communications with the PC
  // Print a message to the LCD.
  int check;             
  while (!Serial);            // Do nothing if no serial port is opened
  finger.begin(57600);        // Init Fingerprint
  Serial.println("Please Scan ");
     
    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
      
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
      getFingerprintIDez();
      
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
  return finger.fingerID; 
}
