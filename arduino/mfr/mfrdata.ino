#include <SPI.h>
#include <MFRC522.h>

#define RFID_RST         8          
#define RFID_SS          9         
MFRC522 rfid(RFID_SS, RFID_RST);
///Checks for an Associated Tag
void asscociatedTag();
//Initializes the RFID Library
void rfidInit();
//Read RFID UID
void readUID();

//Buffer 
char card_buffer[10];
char _tagSwiped=0;

byte readCard[7]; // Array to store UID of a Single Tag temporarily

char* uidBank[] ={
    "A85EE927",
    "C4934A49",
    "B6902EAC"
};

char* assocUID[] ={
    "+254724587654",
    "+254724587654",
    "+254724587654"
};

int availableTags = sizeof(uidBank)/sizeof(uidBank[0]);

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    while(!Serial)
    {
        ;
    }
    rfidInit();
    Serial.println("Setting Up");
	
}

void loop()
{
    readUID();
    // tagSwiped();
    // while(_tagSwiped == 1){
    //     Serial.println("Swiped!");
    //     delay(5000);
    // }
    // // break;
    // if(_tagSwiped == 1)
    // {
    //     Serial.println("Swiped!");
    // }
	
}

void asscociatedTag()
{

}
 void rfidInit()
 {
    pinMode(RFID_SS, OUTPUT);
    digitalWrite(RFID_SS, LOW);
    rfid.PCD_Init();
    rfid.PCD_SetAntennaGain(rfid.RxGain_max);
 }
 void readUID()
 {
    rfidInit();
    // Getting ready for Reading Tags/Cards
    if ( !rfid.PICC_IsNewCardPresent()) {
      return 0; //If a new Access Card is placed to RFID reader continue
    }
    if ( !rfid.PICC_ReadCardSerial()) {
      return 0; //Since a Tag is placed, get Serial and continue
    }
    // Assuming Tags have 4 byte UID, others may have 7 (Reminder)
    Serial.println(F("Scanned Access Tag Unique ID:"));
  
    for (int i = 0; i < 4; i++) {
      readCard[i] = rfid.uid.uidByte[i];
  
      // Operation on lower byte to enable ID in a nibble rather than a byte
      byte lowerByte = (readCard[i] & 0x0F);
      int lowerInt = (int) lowerByte; //converting byte into int
      char lowerChar[1];
      itoa(lowerInt, lowerChar, 16);
      byte upperByte = (readCard[i] >> 4);
      int upperInt = (int) upperByte;
  
      // Operation on upper byte to enable ID in a nibble rather than a byte
      char upperChar[1];
      itoa(upperInt, upperChar, 16);
      int j = i * 2;
      int k = j + 1;
  
      // Storing UID in char array (card_buffer)
      card_buffer[k] = lowerChar[0];
      card_buffer[j] = upperChar[0];
    }
    Serial.print("card buffer : ");
    Serial.println(card_buffer);
    rfid.PICC_HaltA(); // Stop reading
    return 1;
 }
 uint8_t tagSwiped(){
    rfidInit();
    if (rfid.PICC_IsNewCardPresent()) {
         _tagSwiped = 1;
        return 0; //If a new Access Card is placed to RFID reader continue
    }
  }