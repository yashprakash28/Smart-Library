#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10 //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
Servo door; //servo declaration for door

String card_ID = ""; //card UID
/* DATABASE */
String Name1 = "1191760"; //first UID card
String Name2 = "52117164"; //second UID card
String Name3 = "14178131195"; //third UID card
String Name4 = "14813322137"; //fourth UID card
String Name5 = "Your ID"; //
String Name6 = "Your ID"; //and so on.


int NumbCard[6] = {0, 0, 0, 0, 0, 0}; //the number of cards. in my case I have 6 cards.
int j = 0; //increasd by one for every user you add
int statu[6];//the number of cards. in my case I have 6 cards.
int s = 0; //increasd by one for every user you add

//int const RedLed=6;
//int const GreenLed=5;
//int const Buzzer=8;

String Log;
String Name;//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)
int ID = 600;

void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,SEATS_LEFT,Date,Name,Number,Card ID,Time IN,Time OUT");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)
  door.attach(6);
  delay(200);
}

int getID()
{
  //  if ( ! mfrc522.PICC_IsNewCardPresent())
  //  { //If a new PICC placed to RFID reader continue
  //    return 0;
  //  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  { //Since a PICC placed get Serial and continue
    return 0;
  }
  Serial.println(F("Scanned PICC's UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    card_ID += mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA(); // Stop reading
  //  Serial.println("CARD Detected, Details are: ");
  if (card_ID == Name1)
  {
    Name = "SANJANA BANSAL"; //user name
    Number = 17468; //user number
    j = 0; //incresd by one for every user you add
    s = 0; //++1
  }
  else if (card_ID == Name2)  
  {
    Name = "DEEPAK KAMAT"; //user name
    Number = 17427; //user number
    j = 1; //++1
    s = 1; //++1
  }
  else if (card_ID == Name3)
  {
    Name = "NIRMAL KUMAR SINGH"; //user name
    Number = 17451; //user number
    j = 2; //++1
    s = 2; //++1
  }
  else if (card_ID == Name4)
  {
    Name = "HRITIK BANSAL"; //user name
    Number = 17491; //user number
    j = 3; //++1
    s = 3; //++1
  }
  else if (card_ID == Name5)
  {
    Name = "Fifth employee"; //user name
    Number = 789101; //user number
    j = 4; //++1
    s = 4; //++1
  }
  else if (card_ID == Name6)
  {
    Name = "Sixth employee"; //user name
    Number = 789101; //user number
    j = 5; //incresd by one for every user you add
    s = 5; //++1
  }
  //exit statement
  if (NumbCard[j] == 1)
  {
    Serial.println("value of j ");
    Serial.println(j);
    Serial.println(NumbCard[j]);
    NumbCard[j] = 0;
    Serial.println(NumbCard[j]);
    ID = ID + 1;
    Serial.print("DATA,");//send the Name to excel
    Serial.print(ID);
    Serial.print(",");
    Serial.print("DATE");
    Serial.print(",");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number); //send the Number to excel
    Serial.print(",");
    Serial.print(card_ID); //send the card ID to excel
    Serial.print(",");
    Serial.print("");
    Serial.print(",");
    Serial.println("TIME");
    door.write(70);
    delay(2500);
    door.write(0);
  }
  //entry statement
  else if (NumbCard[j] == 0)
  {
    Serial.println("value of j ");
    Serial.println(j);
    Serial.println(NumbCard[j]);
    NumbCard[j] = 1;
    Serial.println(NumbCard[j]);
    ID = ID - 1;
    Serial.print("DATA,");//send the Name to excel
    Serial.print(ID);
    Serial.print(",");
    Serial.print("DATE");
    Serial.print(",");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number); //send the Number to excel
    Serial.print(",");
    Serial.print(card_ID); //send the card ID to excel
    Serial.print(",");
    Serial.print("TIME");
    Serial.print(",");
    Serial.println("");
    door.write(70);
    delay(2500);
    door.write(0);
  }

  else
  {
  }
  //delay(1000);

}
void loop()
{
  door.write(0);
  if (mfrc522.PICC_IsNewCardPresent()) {
    getID();
  }
  Serial.println("NO CARD Detected");
  card_ID = "";
  delay(1000);
}
