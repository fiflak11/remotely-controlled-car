#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); //CE activates transceiver, CSN activates SPI communication

const int btn[] = {2, 3, 4, 5}; //buttons for vehicle control
const char* commands[] = {"12", "21", "11", "22", "00"}; //first digit controls left wheel, second controls right (0-stop, 1-forward, 2-backward)
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening(); //now in transmit mode

  for (int i = 0; i < 4; i++) 
    pinMode(btn[i], INPUT_PULLUP);
}

void loop() {
  const char* text = commands[4]; //if no button is pressed, wheels should stop
  for (int i = 0; i < 4; i++) {
    if (digitalRead(btn[i]) == LOW) {
      text = commands[i]; //set wheel directions based on button pressed
      break;
    }
  }
  radio.write(text, strlen(text) + 1); //send command to vehicle
}

