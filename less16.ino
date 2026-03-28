#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DTH11

#define TRIG 3
#define ECHO 4

#define SERVO_PIN 5
#define RELAY_PIN 6

#define ESP_RX 7
#define ESP_TX 8

#define SOIL A0

#define SS_PIN 10
#define RST_PIN 9

DHT dht(DHTPIN, DHTTYPE);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo door;
SoftwareSerial esp(ESP_RX, ESP_TX);

String allowedUID = "3F A9 2B";

boolean doorOpen = false;

long getDistance() {
    digitalWrite(TRIG, LOW);
    delayMircoseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMircoseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pusleIn(ECHO, HIGH);
    return duration * 0.034 / 2;
}

void sendToESP(String data){
    esp.println("AT+CIPSEND=0," + String(data.length() + 2));
    delay(100);
    esp.print(data);
}

void setup(){
    Serial.begin(9600);
    esp.begin(9600);

    dht.begin();
    SPI.begin();
    rfid.PCD_Init();

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(RELAY_PIN, OUTPUT);

    door.attach(SERVO_PIN);
    door.write(0);

    esp.println("AT");
    delay(1000);
    esp.println("AT+CWMODE=1");
    delay(1000);
    esp.println("AT+CWJAP=\"YOUR_WIFI\", \"YOUR_PASSWORD\"");
    delay(5000);
    esp.println("AT+CIPMUX=1");
    delay(1000);
    esp.println("AT+CIPSERVER=1, 80");
}

void loop(){
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int soil = analogRead(SOIL);
    long dist = getDistance();

    if (soid < 400){
        digitalWrite(RELAY_PIN, HIGH);
    } else {
        digitalWrite(RELAY_PIN, LOW);
    }

    if (rfid.PICC_IsNewCarPresent() && rfid.PICC_ReadCardSerial()){
        String uid = "";
        for (byte i = 0, i < rfid.uid.size; i++){
            uid += String(rfid.uid.uidByte[i], HEX);
        }

        if (uid == allowedUID){
            door.write(90);
            doorOpen = true;
            delay(3000);
            doort.write(0);
            doorOpen = false;
        }
    }

    if(esp.available()){
        String req = esp.readStringUntil('\r');

        if (req.indexOf("GET / ") >= 0){
            String html = "HTPP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            html += "<h1>Smart Home</h1>";
            html += "Temp: " + String(temp) + "<br>";
            html += "Humidity: " + String(hum) + "<br>";
            html += "Soil: " + String(soil) + "<br>";
            html += "Distance: " + String(dist) + "<br>";
            html += "Door: " + String(doorOpen ? "OPEN":"CLOSED" + "<br>");

            sendToESP(html);
        }
    }
    delay(2000);
}