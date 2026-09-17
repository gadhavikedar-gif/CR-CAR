#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <MPU6050.h>

RF24 radio(7, 8);
const byte address[] = "node1";

MPU6050 mpu;

const int switchPin = 10;
int switchState;
int lastState = HIGH;
int mode = 0;

const int x1joy = A2;
const int y1joy = A1;
const int x2joy = A0;
const int y2joy = A3;


struct Data {
  int joyvalx1;
  int joyvaly2;
  int mpuX;
  int mpuY;
  int mode;
};

Data data;

void setup() {

  pinMode(switchPin, INPUT_PULLUP);

  Serial.begin(115200);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  Wire.begin();
  mpu.initialize();

  Serial.println("MPU6050 initialized");
}

void loop() {


  switchState = digitalRead(switchPin);

  
  if (switchState == LOW && lastState == HIGH) {

    mode++;

    if (mode > 3) {
      mode = 1;
    }

    Serial.print("Mode = ");
    Serial.println(mode);

    delay(200);   
  }

  lastState = switchState;


  
  if (mode == 2) {

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    data.mpuX = ax;
    data.mpuY = ay;

    data.joyvalx1 = analogRead(x1joy);

    data.joyvaly2 = analogRead(y2joy);

    data.mode = 2;
  }


 
  else if (mode == 1) {

    data.joyvalx1 = analogRead(x1joy);
    data.joyvaly2 = analogRead(y2joy);

    data.mpuX = 0;
    data.mpuY = 0;

    data.mode = 1;
  }

  else if (mode == 3) {

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    data.mpuX = ax;
    data.mpuY = ay;

    data.joyvalx1 = 0;
    data.joyvaly2 = 0;

    data.mode = 3;
  }

  bool success = radio.write(&data, sizeof(data));

  
  Serial.print("Mode: ");
  Serial.print(data.mode);

  Serial.print(" | X1: ");
  Serial.print(data.joyvalx1);

  Serial.print(" | Y2: ");
  Serial.print(data.joyvaly2);

  Serial.print(" | MPU X: ");
  Serial.print(data.mpuX);

  Serial.print(" | MPU Y: ");
  Serial.print(data.mpuY);

  Serial.print(" | TX: ");
  Serial.println(success ? "OK" : "FAIL");

  delay(20);
}