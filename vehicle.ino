#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); //CE activates transceiver, CSN activates SPI communication
const byte address[6] = "00001";

const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 4;
const int rightBackward = 5;

void setup() {
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //now in receive mode
}

void stopMotors() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, LOW);
}

//11-forward, 22-backward, 12-turn left, 21-turn right
void moveMotors(int left, int right) { //moves wheels in the specified direction
  digitalWrite(leftForward, left == 1 ? HIGH : LOW);
  digitalWrite(leftBackward, left == 2 ? HIGH : LOW);

  digitalWrite(rightForward, right == 1 ? HIGH : LOW);
  digitalWrite(rightBackward, right == 2 ? HIGH : LOW);
}

void loop() {
  if (radio.available()) {
    char text[2] = "";
    radio.read(&text, sizeof(text)); //always receive two digits (00, 11, 12, 21, 22) and store in "text". 00-stop
    stopMotors();
    if(text[0]=='1' && text[1] == '1') //11
      moveMotors(1, 1);
    else if(text[0]=='1' && text[1] == '2') //12
      moveMotors(1, 2);
    else if(text[0]=='2' && text[1] == '1') //21
      moveMotors(2, 1);
    else if(text[0]=='2' && text[1] == '2') //22
      moveMotors(2, 2);
  }
}

