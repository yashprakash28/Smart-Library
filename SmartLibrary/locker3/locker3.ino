
#include <EEPROM.h>     // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices
#include <Servo.h>
#define button1 6
#define button2 7
#define button3 8
//#define button4 8

int locker_id;
String user_id = "";
int locker_no[4] = {1, 2, 3, 4};
int count[4] = {0, 0, 0, 0};
Servo myServo[4];
byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];   // Stores scanned ID read from RFID Module
byte masterCard[4];   // Stores master card's ID read from EEPROM
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(9600);
  SPI.begin(); // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,DATE,TIME,LOCKER_ID,USER_ID,STATUS");// make four columns (Date,Time,locker_id,user_id->user_id gets data from readcard)
  
  for (int i = 0 ; i < EEPROM.length() ; i++)
  {
    EEPROM.write(i, 0);
  }
  for (int i = 0; i < 4; i++)
  {
    myServo[i].attach(2 + i);
    myServo[i].write(0);
  }
  Serial.println("Put your card ");
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  //pinMode(button4,INPUT);
  digitalWrite(button1, LOW);
  digitalWrite(button2, LOW);
  digitalWrite(button3, LOW);
}

String lockout = "";
String dataString = "";
//void convert(){
//  for ( uint8_t i = 0; i < 4; i++)
//  {
//    EEPROM.write(i+25,readCard[i]);
//
//    dataString +=(EEPROM.read(i+25));
//    Serial.print(dataString);
//  }
//}

int getID()
{
  dataString = "";
  //Serial.println("HOLA");
  // Getting ready for Reading PICCs
  //  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  //    return 0;
  //  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  
  Serial.println("card detected");
  Serial.println(F("Scanned PICC's UID:"));
  for ( uint8_t i = 0; i < 4; i++)
  {
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i]);
    dataString += readCard[i];
    Serial.print("  ");
  }
  user_id = dataString;
  Serial.println("  ");

  //  Serial.print(dataString);
  //   Serial.println("enter your book name with coma between them");// get the character
  //  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  
  if (clean(dataString) == 4)
  {
    delay(1000);
    return 0;
  }
  else
  {
    Serial.println("outside clean");
    int j = counter();
    Serial.println(j);
    writeID(j);
  }
  
  Serial.print("DATA,");//send the Name to excel
  Serial.print("DATE");
  Serial.print(",");
  Serial.print("TIME");
  Serial.print(",");
  Serial.print(locker_id);
  Serial.print(",");
  Serial.print(user_id);
  Serial.print(",");
  Serial.print("IN");
  Serial.println(", ");
}

void writeID(int j)
{
  EEPROM.write(j, 1);
  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo[j].write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  for (int i = ((j + 1) * 4), k = 0; i < ((j + 1) * 4) + 4; i++, k++)
  {
    EEPROM.write(i, readCard[k]);
    // EEPROM.read(i);
    // Serial.println(EEPROM.read(0));
  }
  Serial.println("Assigned Locker Number ");
  Serial.println(j + 1);
  locker_id = j + 1;
}

int counter()
{
  for (int i = 0; i < 4; i++)
  {
    if (EEPROM.read(i) == 0)
    {
      Serial.println("inside counter");
      return i;
    }
  }
}

int clean(String ID)
{
  Serial.println("inside clean");
  lockout = "";
  for (int i = 0; i < 4; i++)
  {
    if (EEPROM.read(i) == 1)
    {
      for (int k = ((i + 1) * 4); k < ((i + 1) * 4) + 4; k++)
      {
        lockout += (EEPROM.read(k));
        // delay(500);

        // Serial.println(EEPROM.read(k));
      }
      Serial.print("  ");
      Serial.println(lockout);
      Serial.print("  ");
      Serial.println(ID);
      if (ID == lockout)
      {
        EEPROM.write(i, 0);
       for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo[i].write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
        Serial.println("Removed ID no.");
        Serial.println(lockout);
        Serial.println("Removed locker no. ");
        Serial.println(i + 1);
        locker_id=i+1;

        Serial.print("DATA,");//send the Name to excel
        Serial.print("DATE");
        Serial.print(",");
        Serial.print("TIME");
        Serial.print(",");
        Serial.print(locker_id);
        Serial.print(",");
        Serial.print(user_id);
        Serial.print(",");
        Serial.print("OUT");
        Serial.println(", ");


        Serial.println("Success locker empty");
        return 4;
      }
    }
    lockout = "";
  }
  return 2;
}
int i = 0;

void loop()
{
  //convert();
//  for(byte c=0 ; c<4 ; c++)
//  {
//   myServo[c].write(0);
//  }
////myServo[0].write(0);
//int initial=myServo[0].read();
//Serial.println(initial);
  if (digitalRead( button1) == HIGH)
  {
    Serial.println("Button Pressed");
    for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo[0].write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
//    int angle=myServo[0].read();
//    Serial.println(angle);
    digitalWrite(button1, LOW);
  }
  if (digitalRead( button2) == HIGH)
  {
    Serial.print("Button Pressed");
//        int angle=myServo[1].read();
//    Serial.println(angle);
     for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo[1].write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    myServo[1].write(180);
    digitalWrite(button2, LOW);
  }
  if (digitalRead( button3) == HIGH)
  {
    Serial.print("Button Pressed");
//        int angle=myServo[2].read();
//    Serial.println(angle);
    myServo[2].write(180);
    digitalWrite(button3, LOW);
  }
  /*   if(digitalRead( button4==HIGH))
    {
      myServo[3].write(180);
     }*/

  if (mfrc522.PICC_IsNewCardPresent())
  {
    getID();
  }
  else
  {
    Serial.println("NO card");
  }
  delay(500);
}
