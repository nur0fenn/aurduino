#include "Arduino.h"
#include "ESP8266.h"
#include "dweet.h"
#include "PIR.h"
#include "Servo.h"
#include "PiezoSpeaker.h"

#define ESP_RX 10
#define ESP_TX 11
#define PIR_SIG 4
#define SERVOSM_SIG 3
#define SPEAKER_POS 2

const int servoRestPosition = 20;
const int servoTargetPosition = 150;

unsigned int hoorayLen = 6;
unsigned int hoorayMelody[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
unsigned int hoorayNoteDuration[] = {8,8,8,4,8,4};

unsigned int comeLength = 3;
unsigned int comeMelody[] = {NOTE_C5, NOTE_G5, NOTE_C5};
unsigned int comeNoteDuration[] = {8,8,8};

ESP8266 wifi(ESP_RX, ESP_TX);
Servo servo;
PIR pir(PIR_SIG);
PiezoSpeaker PiezoSpeaker(SPEAKER_POS);

int pirCounter = 0;
const char *SSID = "WIFI-SSID";
const char *PASSWORD = "PASSWORD";

char* const inputToken = "YOURTOKEN_input";
char* const outputToken = "YOURTOKEN_output";

Dweet dweet(&wifi, inputToken, outputToken);

void setup(){
    Serial.begin(9600);
    Serial.println("start");
    if(!wifi.init(SSID,PASSWORD)){
        Serial.println("Wifi init failed.");
        while (true);
    }
    servo.attach(SERVOSM_SIG);
    servo.write(servoRestPosition);
    delay(100);
    servo.detach();
}

void loop(){
    bool pirVal - pir.read();

    dweet.setDweet("pir", pirVal);
    dweet.setDweet("pirCounter", pirCounter);

    dweet.sendDweetKeys();

    if(pirVal){
        pirCounter++;
        servo.attach(SERVOSM_SIG);
        servo.write(servoTargetPosition);
        delay(500);
        servo.write(servoRestPosition);
        delay(500);
        servo.detach();
        PiezoSpeaker.tone(400, 500,);
        delay(500);
        PiezoSpeaker.playMelody(hoorayLen, hoorayMelody, hoorayNoteDuration);
        delay(500);
    }
    dweet.receiveDweetEvents();
    if(strcmp(dweet.getValue(), "servo")==0){
        Serial.println(F("servo was pressed!"));
        servo.attach(SERVOSM_SIG);
        servowrite(servoTargetPosition);
        delay(500);
        servo.write(servoRestPosition);
        delay(500);
        servo.detach();
    }
    else if (strcmp(sweet.getValue(). "playGame") == 0){
        Serial.println(F("Playing Game!"));
        while(!pir.read()){
            PiezoSpeaker.playMelody(comeLength, comeMelody, comeNoteDuration);
            delay(500);   
        }
        servo.attach(SERVOSM_SIG);
        servo.write(servoTargetPosition);
        delay(1000);
        servo.write(servoRestPosition);
        delay(1000);
        servo.detach();
        PiezoSpeaker.playMelody(comeLength, comeMelody, comeNoteDuration);
        delay(100);
    }
}