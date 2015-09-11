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

#define BUTTON_PIN 2
#define RED_PIN    3
#define GREEN_PIN  4
#define BLUE_PIN   5
#define BUZZER_PIN 6
#define RST_PIN    9
#define SS_PIN    10

int button_state;
char incoming_data;

bool button_was_pressed = false;
bool timed_out = false;
bool quit = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
}

void loop() {
  if (!quit) {
    //Turn LED blue
    setColor(0, 0, 255);

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
      button_was_pressed = true;
    } else {
      //Output the UID over the serial cable
      mfrc522.PICC_OutputUID(&(mfrc522.uid));
    }

    unsigned long start_time = millis();
    bool valid = false;
    while (!valid) {
      while ((Serial.available() < 1) && !timed_out) {
        if (!button_was_pressed && ((millis() - start_time) > 2000)) timed_out = true;
      }
      if (!timed_out) {
        incoming_data = Serial.read();
        if (incoming_data != '9') valid = true; 
      } else {
        incoming_data = '2';
        valid = true;
      }
    }
    
    int wait_time = 1000;
    switch (incoming_data) {
      //Logged
      case '0':
        //Turn LED green
        //High-pitched beep
        setColor(0, 255, 0);
        tone(BUZZER_PIN, 3000, 400);
        break;

      //Error
      case '1':
        //Turn LED red
        //Low-pitched beep
        setColor(255, 0, 0);
        tone(BUZZER_PIN, 500, 400);
        break;

      //Quitting
      case '2':
        //Turn LED yellow
        setColor(255, 140, 0);
        wait_time = 1500;
        tone(BUZZER_PIN, 200, wait_time);
        quit = true;
        break;

      //Bug/Should not happen
      default:
        //Turn LED white
        setColor(255, 255, 255);
        wait_time = 3000;
        tone(BUZZER_PIN, 5000, wait_time);
        break;
    }
    button_was_pressed = false;
    delay(wait_time);
  } else {
    //Turn off LED
    setColor(0, 0, 0);
    while (Serial.available() < 1) {}
    incoming_data = Serial.read();
    if (incoming_data == '9') {
      quit = false;
      timed_out = false;
      delay(500);
    }
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
