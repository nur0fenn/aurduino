#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <Servo.h>

#define SERVO_PIN 9
#define APDS9960_PIN 2
#define RED_LED A1
#define GREEN LED A2

const int passwordLen = 6;

Servo servo;

uint8_t ptoximityData = 0;
int pageState = 0;
String password[passwordLen] = {"UP", "DOWN", "UP", "UP", "RIGHT", "LEFT"};
String recognizedGesture = "";
int passCounter = 0;

SparkFun_APDS9960 apds = SparkFun_APDS9960();
int flag = 0;

// Your code here

void setup() {
  pinMode(APDS9960_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(0);
  servo.detach();

  Serial.begin(9600);
  Serial.print("Password length: ");
  Serial.print(passwordLen);
  Serial.println();
  Serial.println("**************");
  Serial.println("APDS-9960 Lock");
  Serial.println("**************");

  attachInterrupt(0, interruptRoutine, FALLING);
  // Your code here

  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init()) {
    Serial.println("APDS-9960: Initialization APDS OK!");
  } else {
    Serial.println("APDS-9960: Initialization APDS ERROR!");
  }

  if (apds.setProximityGain(PGAIN_1X)) {
    Serial.println("PGAIN: Set proximity gain OK!");
  } else {
    Serial.println("PGAIN: Set proximity gain ERROR!");
  }

  if (apds.enableProximitySensor(false)) {
    Serial.println("Proximity Sensor: Start proximity sensor OK!");
  } else {
    Serial.println("Proximity Sensor: Start proximity sensor ERROR!");
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true)) {
    Serial.println("Gesture Sensor: Start gesture sensor OK!");
  } else {
    Serial.println("Gesture Sensor: Start gesture sensor ERROR!");
  }
}

void loop() {
  if (pageState == 0){
    if(!apds.readProximity(proximityData)){
      Serial,println("error reading proximity value");
    } else{
      Serial.println(proximityData);
      if ((proximityData >= 40) && (pageState == 0)){
        pageState = 1;
        delay(2000);
      }
    }
  }

  switch(pageState){
    case 0:
      blinkRed();
      break;

    case 1:
      passCounter = 0;
      for (int x = 0; x < passwordLen; x++) {
        Serial.println(x);
        while (apds.isGestureAvailable() == false){

        }
        if(flag == 1){
          detachInterrupt(0);
          handleGesture();
          flag = 0;
          attachInterrupt(0, interruptRoutine, FALLING);
          if (recognizedGesture != "NONE"){
            if (recognizedGesture == password[x]){
              passCounter += 1;
              analogWrite(GREEN_LED, 255);
              delay(1000);
              turnOff();
            }
            else if (recognizedGesture !- password[x]) {
              analogWrite(RED_LED, 255);
              delay(1000);
              turnOff();
              pageState = 0;
              break;
            }
            if (passCounter == passwordLen){
              pageState = 2;
              break;
            }
          }
          else if (recognizedGesture == "NONE"){
            x -= 1;
          }
        }
      }
    break;

    case 2:
      pageState = 0;
      servo.attach(SERVO_PIN);
      servo.write(90);
      enter();
      delay(1000);
      servo.write(0);
      break;
  }
}

void interruptRoutine(){
  flag = 1;
}

void turnOff(){
  analogWrite(RED_LED, 0);
  analogWrite(GREEN_LED, 0);
}

void blinkRed(){
  analogWrite(RED_LED, 255);
  delay(250);
  turnOff();
  delay(250);
}

void enter(){
  for (int x=0; x<30; x++){
    analogWrite(GREEN_LED, 255);
    delay(100);
    turnOff();
    delay(100);
  }
  for (int = 9; x>0; x--){
    delay(1000);
  }
}

void handleGesture(){
  if (apds.isGestureAvailable()){
    switch (apds.readGesture()){
      case DIR_UP :
        Serial.println("UP");
        recognizedGesture = "UP";
        break;
      case DIR_DOWN :
        Serial.println("DOWN");
        recognizedGesture = "DOWN";
        break;
      case DIR_LEFT :
        Serial.println("LEFT");
        recognizedGesture = "LEFT";
        break;
      case DIR_RIGHT :
        Serial.println("RIGHT");
        recognizedGesture = "RIGHT";
        break;
      case DIR_NEAR :
        Serial.println("NEAR");
        recognizedGesture = "NEAR";
        break;
      case DIR_FAR :
        Serial.println("FAR");
        recognizedGesture = "FAR";
        break;
      default:
        Serial.println("NONE");
        recognizedGesture = "NONE";
    }
  }
}