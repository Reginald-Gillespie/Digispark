/*

Most of this is commented out failed code, but hey it works. The remote for our stero sound system went out so I made this to control the volume on the computer dirrectly.
`up` and `down` are the remote codes, you'll need to print the received command to figure out what that is for the buttons you want to use.
In decodeIRSignal() on lines 83 and 89, the value 32 might change depending on remote you're using. If your remote spams random values that are not even close to the same, chances are you have this number wrong.

*/



// #define DECODE_SAMSUNG
// #include "TinyIRReceiver.hpp"

// #include "DigiUSB.h"

// #define CUSTOM_REPORT { 0x05, 0x0c,  0x09, 0x01, 0xa1, 0x01, 0x85, 0x01, 0x19, 0x00, 0x2a, 0x3c, 0x02, 0x15, 0x00, 0x26, 0x3c, 0x02, 0x95, 0x01, 0x75, 0x10, 0x81, 0x00, 0xc0 }

// #include "DigiKeyboard.h"

// #define USB_CFG_DEVICE_NAME     'D','i','g','i','I','R
// #define USB_CFG_DEVICE_NAME_LEN 6

#include "TrinketHidCombo.h"

#define IR_PIN 0
#define GND 1
#define VCC 2

#define up 49215
#define down 41055

#define KEY_MUTE 127
#define KEY_VOLUME_UP 128
#define KEY_VOLUME_DOWN 129


unsigned long receivedValue;

void setup() {
  // DigiUSB.begin();
  // DigiUSB.delay(500);
  // DigiUSB.println("Starting");

  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  // pinMode(GND, INPUT);

  pinMode(IR_PIN, INPUT);

  TrinketHidCombo.begin();
  // DigiKeyboard.sendKeyStroke(0);

  // DigiUSB.delay(1000);
}

// Function to check if an IR signal has been received
bool irSignalReceived() {
  unsigned long startTime = millis();
  while (digitalRead(IR_PIN) == HIGH) {
    
    if (millis() - startTime > 200) {
      // Timeout reached, no signal received
      return false;
    }
  }
  return true;

  // DigiKeyboard.println(digitalRead(IR_PIN));
  // return false;
}

// Function to decode the received IR signal using Sony SIRC protocol
unsigned int decodeIRSignal() {
  // DigiUSB.refresh();
  unsigned int receivedValue = 0;
  
  // Skip the start pulse (2.4 ms)
  pulseIn(IR_PIN, LOW);
  
  // Read the subsequent 12 pulses (0.6 ms each)
  for (int i = 0; i < 32; i++) {
    // DigiUSB.refresh();
    unsigned long pulseDuration = pulseIn(IR_PIN, HIGH);
    
    // Check the pulse duration to determine the bit value
    if (pulseDuration > 1000) {
      receivedValue |= (1UL << (31 - i));
    }
  }
  
  return receivedValue;
}


// int pingEvery = 8;
// int lastPing = millis();

void loop() {
  TrinketHidCombo.poll();
  // DigiUSB.refresh();

  if (irSignalReceived()) {
    // DigiUSB.println("Signal received");
    // DigiKeyboard.println("Signal received");

    unsigned int receivedCommand = decodeIRSignal();
    // DigiKeyboard.println(receivedCommand);
    // DigiKeyboard.delay(180);

    if (receivedCommand - 2 < up && receivedCommand + 2 > up) {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
      // DigiKeyboard.println("UP");
      // DigiKeyboard.sendKeyStroke(80);
    } else if (receivedCommand - 2 < down && receivedCommand + 2 > down) {
      //Bring vol down twice as fast
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      // DigiKeyboard.println("Down");
      // DigiKeyboard.sendKeyStroke(81);
    }
    
    TrinketHidCombo.println(receivedCommand);
    for(int i = 0; i < 11; i++) {
      TrinketHidCombo.poll();
      delay(9);
    }


    // DigiUSB.println(receivedCommand);
  }

  // if (millis() - lastPing > pingEvery) {
  //   lastPing = millis();
  //   // DigiKeyboard.update();
  //   // DigiUSB.println("Ping");
  //   // DigiKeyboard.println("Ping");
  //   // TrinketHidCombo.poll();
  // }

  // int randomPin = random(3);
  // int randomState = random(2);
  // digitalWrite(randomPin, randomState);

  // DigiUSB.delay(2000);
  // DigiUSB.println("Ping");
}


