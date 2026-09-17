#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7, 8);
const byte address[] = "node1";



struct Data {
  int joyvalx1;
  int joyvaly2;
  int mpuX;
  int mpuY;
  int mode;
};

Data data;


// Motor pins
const int motorA1 = 4;
const int motorA2 = 2;

const int motorB1 = 9;
const int motorB2 = 5;

const int pwmA = 3;
const int pwmB = 6;

int speedx1;
int speedy1;

void stopMotors() {

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);

  
}


void forward() {

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

 // digitalWrite(pwmA, map(data.joyvalx1, 519, 1023, 0, 255));
  //analogWrite(pwmB, map(data.joyvalx1, 519, 1023, 0, 255));


 // analogWrite(pwmA, speedx1);
  //analogWrite(pwmA, speedy1);
  //analogWrite(pwmB, 255);


}


void backward() {

  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);

  //digitalWrite(pwmA, map(data.joyvalx1, 519, 1023, 0, 255));
 // analogWrite(pwmB, map(data.joyvalx1, 519, 1023, 0, 255));

}


void left() {

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);

 // digitalWrite(pwmA, map(data.joyvaly2, 519, 1023, 0, 255));
  //analogWrite(pwmB, map(data.joyvaly2, 519, 1023, 0, 255));

}


void right() {

  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  //digitalWrite(pwmA, map(data.joyvaly2, 519, 1023, 0, 255));
//  analogWrite(pwmB, map(data.joyvaly2, 519, 1023, 0, 255));

}


void setup() {

  Serial.begin(115200);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);

  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);

  analogWrite(pwmA, 255);
  analogWrite(pwmB, 255);

  //analogWrite(pwmA, map(data.joyvaly2, 519, 1023, 0, 255));
  //analogWrite(pwmB, map(data.joyvaly2, 519, 1023, 0, 255));

 //speedx1= map(data.joyvalx1, 519, 1023, 0, 255);
  //speedx1 = map(constrain(data.joyvalx1, 519, 1023), 519, 1023, 0, 255);
 // speedy1= map(data.joyvalx1, 519, 0, 0, 255);

  stopMotors();

  Serial.println("Receiver ready");
}


void loop() {

  if (radio.available()) {

    
    radio.read(&data, sizeof(data));


  
   Serial.print("Mode = ");
    Serial.print(data.mode);

    Serial.print(" | X1 = ");
    Serial.print(data.joyvalx1);

    Serial.print(" | Y2 = ");
    Serial.print(data.joyvaly2);

    Serial.print(" | MPU X = ");
    Serial.print(data.mpuX);

    Serial.print(" | MPU Y = ");
    Serial.println(data.mpuY);
    

   // Serial.print(" | X1 = ");
    //Serial.print(data.joyvalx1);

   // Serial.print(" | speedx1 = ");
    //Serial.println(speedx1);


    
    if (data.mode == 1) {

      
      if (data.joyvalx1 < 500) {
        forward();
      }

      else if (data.joyvalx1 > 530) {
       backward();
      }

      else if (data.joyvaly2 < 500) {
      left();
      }

      else if (data.joyvaly2 > 530) {
      right();
      }

      else {

        stopMotors();
      }
    }


    else if (data.mode == 3) {

      if (data.mpuX > 5000){
        right();
      }

       else if (data.mpuX<-5000){
       left();
       }

       else if (data.mpuY > 5000){
       backward();
      }

       else if (data.mpuY<-5000){
        forward();

       }
      
       else{
      stopMotors();
       }
    }


   

    else if (data.mode == 2) {

      
      if (data.mpuX > 5000){
         
         right();
      }
       else if (data.mpuX<-5000){
       
       
        left();
       }
       else if (data.joyvalx1 < 500) {

        forward();
      }

      else if (data.joyvalx1 > 600) {

        backward();
      }
      
       else{
      stopMotors();
       }

      
    }

   
  }
}