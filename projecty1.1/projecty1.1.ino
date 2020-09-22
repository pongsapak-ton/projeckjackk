#include <Adafruit_Fingerprint.h>
#include <UTFT.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);


const char* ssid = "CD_STUDENT"; //ใส่ชื่อ SSID Wifi
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
 #define  BLACK   0x0000
  #define BLUE    0x001F
  #define RED     0xF800
  #define GREEN   0x07E0
  #define CYAN    0x07FF
  #define MAGENTA 0xF81F
  #define YELLOW  0xFFE0
  #define WHITE   0xFFFF
  #define GREY    0x8410
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
UTFT myGLCD(CTE32HR,38,39,40,41);
uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
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
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
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

uint8_t readnumber(void) {
 
char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
  return key;
}
uint8_t fun(void){
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
  return key;
}
int deletefin(){
  char key;
   Serial.println("\n\nDelete Finger");
   Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
   uint8_t id = fun();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);
  Serial.println(fun());
  deleteFingerprint(id);
}

int addfigerprint(){
  myGLCD.setBackColor(YELLOW);
  myGLCD.setColor(CYAN);
  myGLCD.fillRoundRect (2,105,475,315); 
  myGLCD.drawRoundRect (1,110,475, 315);
  myGLCD.setColor(BLUE);
  myGLCD.print("* MENU ADD FINGERPRINT  *", CENTER, 134);
  myGLCD.print("   * Ready to enroll  *", CENTER, 164);
  myGLCD.print("   * a fingerprint! *", CENTER , 184);
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");

  id = readnumber();
 
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
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
   p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}


int sendt(){
     
      myGLCD.setBackColor(YELLOW);
      myGLCD.setColor(CYAN);
      myGLCD.fillRoundRect (2,105,475,315); 
      myGLCD.drawRoundRect (1,110,475, 315);
      myGLCD.setColor(BLUE);
      myGLCD.print("* MENU FINGERPRINT  *", CENTER, 134);
      myGLCD.print(" * READY TO GET FINGERPRINT *", LEFT, 164);
       getFingerprintID();
       getFingerprintID();
       getFingerprintID();
       getFingerprintID();
       getFingerprintID();
       getFingerprintID();
    
      
}
int Select(){
    char key = fun();
    if (key == 'A') {
    addfigerprint();
   
  }
   if (key == 'B'){
         sendt();
  }
   if (key == "C" ){
     deletefin();
  }
  if (key == 'D'){
     homes();
  } 
}
int homec(){
  myGLCD.setBackColor(YELLOW);
  myGLCD.setColor(CYAN);
  myGLCD.fillRoundRect (5, 7,475,95); 
  myGLCD.drawRoundRect (1,5,475,100);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(BLACK);
  myGLCD.print("* CHITRALADA*", CENTER, 14);
  myGLCD.print("* TECHNOLOGY INSTITUTE *", CENTER, 34);
  myGLCD.print("* FRIDAY 28/09/63 *", CENTER, 55);
  myGLCD.print("* 12:51 *", CENTER, 74);
}
  int homes(){
    myGLCD.setBackColor(YELLOW);
  myGLCD.setColor(CYAN);
  myGLCD.fillRoundRect (2,105,475,315); 
  myGLCD.drawRoundRect (1,110,475, 315);
  myGLCD.setColor(BLUE);
  myGLCD.print("* MENU *", CENTER, 134);
  myGLCD.print("   *'A' ADD FINGERPRINT*", LEFT, 164);
  myGLCD.print("   *'B' INPUT*",LEFT, 194);
  myGLCD.print("   *'C' DELETE FINGERPRINT *", LEFT, 224);
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
    while (!Serial);  // For Yun/Leo/Micro/Zero/...
    delay(100);
    Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
   myGLCD.InitLCD();
    // set the data rate for the sensor serial port
    finger.begin(57600);
  
      if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } else {
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }
     finger.getTemplateCount();
  
    if (finger.templateCount == 0) {
      Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    } 
    else {
      Serial.println("Waiting for valid finger...");
        Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    }
    homec();
   
  
}
  
void loop() {
  
  // put your main code here, to run repeatedly:
   char key;
   int num;

Select();

  
  
  }  
 
