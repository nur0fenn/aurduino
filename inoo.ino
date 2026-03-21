#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(10, 9);
Servo sg90;

constexpr uint8_t greenLed = 7;
constexpr uint8_t redLed = 6;
constexpr uint8_t servoPin = 8;
constexpr uint8_t buzzerPin = 5;
string tagUID = "29 B9 ED 23";

void setup(){
    pinMode(buzzerPin, OUTPUT);
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);

    sg90.attach(servoPin);
    sg90.write(0);
    lcd.begin();
    lcd.backlight();
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.clear();
}

void loop(){
    lcd.setCursor(0, 0);
    lcd.print("RFID Door Lock");
    lcd.setCursor(0, 1);
    lcd.print("Show Your Tag");

    if (!mfrc522.PICC_IsNewCardPresent()){
        return;
    }
    
    if (!mfrc522.PICC_IsNewCardSerial()){
        return;
    }

    String tag = "";
    for(byte i = 0; i < mfrc522.uid.size; i++){
        tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.tiUpperCase();

    if(tag.substring(1) == tagUID){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        lcd.setCursor(0, 1);
        lcd.print("Door opened");
        sg90.write(90);
        digitalWrite(greenLed, HIGH);
        delay(3000);
        digitalWrite(greenLed, LOW);
        sg90.write(0);
        lcd.clear();
    }

    esle{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Tag Shown");
        lcd.setCursor(0, 1);
        lcd.print("Access Denied");
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(redLed, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(redLed, LOW);
        lcd.clear();
    }
}