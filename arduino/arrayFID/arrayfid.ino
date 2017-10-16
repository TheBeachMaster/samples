#include <SPI.h>      //Library  RC522 Module uses SPI protocol
#include <MFRC522.h> //Library  RC522 Module
#define RFID_RST         8
#define RFID_SS          9
MFRC522 rfid(RFID_SS, RFID_RST);
int successRead; // Variable integer to keep if we have Successful Read from Reader
byte storedCard[7];   // Stores an ID read from EEPROM
byte readCard[7];           // Stores scanned ID read from RFID Module

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
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    SPI.begin();           // MFRC522 Hardware uses SPI protocol
    rfid.PCD_Init();    // Initialize MFRC522 Hardware
    rfid.PCD_SetAntennaGain(rfid.RxGain_max);

}

void loop()
{
	
}

int getCardUID()
{
    // Getting ready for Reading PICCs
  if ( ! rfid.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! rfid.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return 0;
  }
}