/*This code created by Electronic Tech H.I YouTube Channel 
 * 
 *First scan your cards using this code and the ID will appear on the Serial monitor
 *by this command Serial.println(card_ID) just uncomment it, then set the IDs that you scanned to any name you want
 *that means you don't have to upload another code to scan the card IDs.
*/
#include <SPI.h>
#include <MFRC522.h>
//#include <LiquidCrystal.h>// include the library code:

//LiquidCrystal lcd(2);  //(or the latch pin of the 74HC595)

#define SS_PIN 10 //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

String card_ID=""; //card UID

//Add as many cards you want 
String Name1="052117164";//first UID card
String Name2="7322718241";//second UID card
String Name3="1191760";//third UID card
String Name4="14813322137";//fourth UID card
String Name5="Your ID";//
String Name6="Your ID";//and so on.

int NumbCard[6];//the number of cards. in my case I have 6 cards.
int j=0;  //incresd by one for every user you add
int statu[6];//the number of cards. in my case I have 6 cards.
int s=0;  //incresd by one for every user you add 

int const RedLed=6;
int const GreenLed=5;
int const Buzzer=8;

String Log;
String Name;//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)  
int ID=600;
void setup() {
  //lcd.begin(16, 2);
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,ID,Date,Name,Number,Card ID,Time IN,Time OUT");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  
  //lcd.setCursor(2,0);
  //lcd.print("Good Morning");//Wlecome Message will appear in the beginning. 
  delay(200);
   }
   
void loop() {
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }

 for (byte i = 0; i < mfrc522.uid.size; i++) 
 {
     card_ID += mfrc522.uid.uidByte[i];
 
 }
 //Serial.println(card_ID);//Uncomment this line to scan the card ID and appear it on Serial monitor.
 
       if(card_ID==Name1){
       Name="SANJANA BANSAL";//user name
       Number=17468;//user number
       j=0;//incresd by one for every user you add
       s=0;//++1
      }
      else if(card_ID==Name2){
       Name="DEEPAK KAMAT";//user name
       Number=17427;//user number
       j=1;//++1
       s=1;//++1
      }
      else if(card_ID==Name3){
       Name="NIRMAL KUMAR SINGH";//user name
       Number=17451;//user number
       j=2;//++1
       s=2;//++1
      }
      else if(card_ID==Name4){
       Name="HRITIK BANSAL";//user name
       Number=17491;//user number
       j=3;//++1
       s=3;//++1
      }
      else if(card_ID==Name5){
       Name="Fiveth employee";//user name
       Number=789101;//user number
       j=4;//++1
       s=4;//++1
      }
      else if(card_ID==Name6){
       Name="Sixth employee";//user name
       Number=789101;//user number
       j=5;//incresd by one for every user you add
       s=5;//++1
      }
      else{
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
          goto cont;//go directly to line 188
     }

      if(NumbCard[j] == 1)
      {
      NumbCard[j] == 0;
     //statu[s]=1;
     // lcd.clear();
     // lcd.setCursor(0,0);
     // lcd.print("Good Bye");
     // lcd.setCursor(0,1);
     // lcd.print(Name);
      ID=ID+1;
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
     
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      digitalWrite(Buzzer,HIGH);
      delay(30);
      digitalWrite(Buzzer,LOW);
      }
      else if(NumbCard[j] == 0){
      NumbCard[j] = 1;
      
      n++;//(optional)
     // lcd.clear();
     // lcd.setCursor(4,0);
     // lcd.print("Welcome ");
     // lcd.setCursor(0,1);
     // lcd.print(Name);
     ID=ID-1;
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
      digitalWrite(GreenLed,HIGH);
      digitalWrite(RedLed,LOW);
      digitalWrite(Buzzer,HIGH);
      delay(30);
      digitalWrite(Buzzer,LOW);
      
      }
      else {
     //lcd.clear();
     //lcd.setCursor(0,0);
     //lcd.print(Name);
     //lcd.setCursor(0,1);
     //lcd.print("Already Left");
      }
      //delay(1000);
      //lcd.clear();
      //lcd.setCursor(2,0);
      //lcd.print("Good Morning");

cont:
delay(2000);
digitalWrite(GreenLed,LOW);
digitalWrite(RedLed,LOW);
card_ID="";

//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
/*if(n==2){
    Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
    Serial.println("FORCEEXCELQUIT");
    }*/
}
    
