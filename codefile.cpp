#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define S1 8
#define S2 7
#define S3 6
#define S4 5
#define S5 4

int vote1 = 0;
int vote2 = 0;
int vote3 = 0;
int vote4 = 0;

uint8_t id;

void setup() {
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  Wire.begin();
  finger.begin(57600);

  lcd.init();
  lcd.backlight();

  lcd.print("Smart Electronic ");
  lcd.setCursor(0, 1);
  lcd.print(" Voting Machine ");
  delay(4000);

  digitalWrite(S1, HIGH);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, HIGH);
  digitalWrite(S5, HIGH);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("A");
  lcd.setCursor(5, 0);
  lcd.print("B");
  lcd.setCursor(9, 0);
  lcd.print("C");
  lcd.setCursor(13, 0);
  lcd.print("D");

  // Fingerprint enrollment setup
  Serial.begin(9600);
  while (!Serial); // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  // Call the enrollFingerprint function here
  enrollFingerprint();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("BJP");
  lcd.setCursor(0, 1);
  lcd.print(vote1);
  lcd.setCursor(4, 0);
  lcd.print("INC");
  lcd.setCursor(5, 1);
  lcd.print(vote2);
  lcd.setCursor(9, 0);
  lcd.print("BSP");
  lcd.setCursor(9, 1);
  lcd.print(vote3);
  lcd.setCursor(13, 0);
  lcd.print("AAP");
  lcd.setCursor(13, 1);
  lcd.print(vote4);

  // Reset fingerprint verification status after each vote
  bool fingerprintVerified = false;

  if (digitalRead(S1) == 0 && !fingerprintVerified) {
    vote1++;
    fingerprintVerified = checkFingerprint();
    while (digitalRead(S1) == 0);
  }

  if (digitalRead(S2) == 0 && !fingerprintVerified) {
    vote2++;
    fingerprintVerified = checkFingerprint();
    while (digitalRead(S2) == 0);
  }

  if (digitalRead(S3) == 0 && !fingerprintVerified) {
    vote3++;
    fingerprintVerified = checkFingerprint();
    while (digitalRead(S3) == 0);
  }

  if (digitalRead(S4) == 0 && !fingerprintVerified) {
    vote4++;
    fingerprintVerified = checkFingerprint();
    while (digitalRead(S4) == 0);
  }

  if (digitalRead(S5) == 0) {
    int vote = vote1 + vote2 + vote3 + vote4;
    if (vote) {
      if ((vote1 > vote2 && vote1 > vote3 && vote1 > vote4)) {
        lcd.clear();
        lcd.print("A is Winner");
        delay(3000);
        lcd.clear();
      } else if ((vote2 > vote1 && vote2 > vote3 && vote2 > vote4)) {
        lcd.clear();
        lcd.print("B is Winner");
        delay(3000);
        lcd.clear();
      } else if ((vote3 > vote1 && vote3 > vote2 && vote3 > vote4)) {
        lcd.clear();
        lcd.print("C is Winner");
        delay(3000);
        lcd.clear();
      } else if (vote4 > vote1 && vote4 > vote2 && vote4 > vote3) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("D is Winner");
        delay(3000);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.print(" No Result ");
        delay(3000);
        lcd.clear();
      }
    } else {
      lcd.clear();
      lcd.print("No Votes Casted");
      lcd.setCursor(6, 1);
      lcd.print("Yet");
      delay(3000);
      lcd.clear();
    }
    vote1 = 0;
    vote2 = 0;
    vote3 = 0;
    vote4 = 0;
    fingerprintVerified = false; // Reset fingerprint verification status
    lcd.clear();
  }
}
bool checkFingerprint() {
  lcd.clear();
  lcd.print("Place your finger");
  enrollFingerprint();
  while (!finger.getImage());
  int fingerprintID = finger.fingerFastSearch();

  if (fingerprintID != FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.print("Already Voted!");
    delay(2000);
    lcd.clear();

    // Check if a vote has been cast
    if (vote1 > 0 || vote2 > 0 || vote3 > 0 || vote4 > 0) {
      lcd.clear();
      lcd.print("Enter new fingerprint ID:");
      fingerprintID = readnumber();
    }

    return true;
  } else {
    lcd.clear();
    lcd.print("Fingerprint Matched!");
    delay(2000);
    lcd.clear();
    return false;
  }
}


uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (!Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void enrollFingerprint() {
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (!getFingerprintEnroll());
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for a valid finger to enroll as #"); Serial.println(id);

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
  Serial.println("Place the same finger again");
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
  Serial.print("Creating model for #"); Serial.println(id);
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
}
