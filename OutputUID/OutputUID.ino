/*
 * ------------------------------------------------------------------------------------------------------------------
 * Reads the UID of the tag being scanned through the reader and outputs it to the computer through the serial cable.
 * 
 * Uses the MFRC522 library; See https://github.com/miguelbalboa/rfid
 * Board used: SainSmart Arduino UNO
 * Reader Used: Sunfounder RC522
 * 
 * 
 * Pin Layout:
 * ---------------------------------
 * Reader Pin     Arduino Pin Used
 * ---------------------------------
 * NSS            10
 * SCK            13
 * MOSI           11
 * MISO           12
 * GND            GND
 * RST            9
 * VCC            3.3V
 */

#include <SPI.h>
#include "MFRC522.h"

#define LED_PIN    2
#define BUTTON_PIN 3
#define RST_PIN    9 
#define SS_PIN    10

int button_state;
char incoming_data;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
}

void loop() {
  //Turn LED blue
  digitalWrite(LED_PIN, LOW);

  button_state = digitalRead(BUTTON_PIN);
  
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() && button_state == LOW) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial() && button_state == LOW) {
    return;
  }

  
  if (button_state == HIGH) {
    //Indicate that the button was pressed over the serial cable
    Serial.print("FFFFFFFF");
    //Turn LED blue
  } else {
    //Output the UID over the serial cable
    mfrc522.PICC_OutputUID(&(mfrc522.uid));
  }

  while (Serial.available() < 1) {}
  incoming_data = Serial.read();

  switch(incoming_data) {
    //Logged
    case '0':
      //Turn LED green
      //High-pitched beep
      digitalWrite(LED_PIN, HIGH);
      break;
      
    //Error
    case '1':
      //Turn LED red
      //Low-pitched beep
      break;
      
    //Quitting
    case '2':
      //Turn LED orange
      break;
      
    //Bug/Should not happen
    default:
      //Turn LED yellow
      break;
  }
  delay(1000);
}
