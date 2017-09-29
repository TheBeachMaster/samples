#include <SPI.h>
#include <MFRC522.h>

#define RFID_RST         8          
#define RFID_SS          9         
MFRC522 rfid(RFID_SS, RFID_RST);
///Checks for an Associated Tag
int asscociatedTag(char cardNum, char* cardBank[],char* assocNum[]);
//Initializes the RFID Library
void rfidInit();
//Read RFID UID
void readUID();

//Buffer 
char card_buffer[10];
char _tagSwiped=0;

byte readCard[7]; // Array to store UID of a Single Tag temporarily

char* uidBank[] ={
    "a85ee927", //A85EE927
    "c4934a49", //C4934A49
    "b6902eac" //B6902EAC
};

char* assocUID[] ={
    "+254724587654",
    "+254712384171",
    "+254703415953"
};

uint8_t availableTags = sizeof(uidBank)/sizeof(uidBank[0]);
int isCardAvailable(char* storageBank[],char cardVal[10]);



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
 int isCardAvailable(char* storageBank[],char cardVal[10])
 {
    int i = 0;
    char* tagsBank[100];
    char cardNumber[10];
    storageBank = tagsBank;
    cardVal = cardNumber ;
    int tags = sizeof(storageBank)/sizeof(storageBank[0]);
     for (i; i < tags; i++)
     {
         if (tagsBank[i] == cardNumber)
         {
             return tags;
         }
     }
     if (i == availableTags)
     {
        return -1;
     }
    
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
    int response = isCardAvailable(uidBank,card_buffer);
    Serial.println(response);
    // if (isCardAvailable(uidBank,card_buffer))
    // {
    //     Serial.println("Found it");
    // }else
    // {
    //     Serial.println("Not found");
    // }
    // // int i = 0;

    // for (i; i < availableTags; i++)
    // {
    //     if (uidBank[i] == card_buffer)
    //     {
    //         Serial.println("Found");
    //     }
    // }
    // if (i == availableTags)
    // {
    //     Serial.println("Not found");
    // }
    // if (uidBank[0] == card_buffer)
    // {
    //     Serial.println("Found");
    
    // }
   
    // if (card_buffer == uidBank[i])
    // {
    //     Serial.println("Found!");
       
    // }
    // for (int i = 0; i < availableTags; i++)
    // {
    //     // Serial.print(F("Bank number "));
    //     // Serial.print(i);
    //     // Serial.print(F(" : "));
    //     // Serial.print(uidBank[i]);
    //     // Serial.print(F("is associated with : "));
    //     // Serial.println(assocUID[i]);
    //     if (card_buffer == uidBank[i])
    //     {
    //         Serial.print("Card Found at");
    //         Serial.println(uidBank[i]);
    //        // return 0;
    //     }
    // }
    rfid.PICC_HaltA(); // Stop reading
    return 0;
 }
 uint8_t tagSwiped(){
    rfidInit();
    if (rfid.PICC_IsNewCardPresent()) {
         _tagSwiped = 1;
        return 0; //If a new Access Card is placed to RFID reader continue
    }
  }