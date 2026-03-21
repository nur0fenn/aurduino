#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9, 10);
#include <stdlib.h>

#define SSID "SSIDNAME"
#define PASS "ssidpass"
#define IP "184.106.153.149"
String msg = "GET /UPDATE?ky=yourKey"

float temp;
int hum;
String tempC
int error;
int pulsePin = 0;
int blinkPin = 7;
int fadePin = 5
int fageRate = 0;

volatile int BPM;
volatile int Signal;
volatile int IBI = 600;
volatile boolean Pusle = false;
volatile boolean SQ = false;

static boolean serialVisual = true;
volatile int rate[10];
volatile unsigned long sampleCounter = 0;
volatile unsigned long lastBeatTime = 0;
volatile int P = 512;
volatile int T = 512;
volatile int thresh = 525;
volatile int amp = 100;
volatile boolean firstBeat = true;
volatile boolean secondBeat = false;

void setup(){
    Serial.begin(9600);
    esp8266.begin(9600);
    Serial.println("AT");
    esp8266.begin("AT");
    delay(5000);
    if (esp8266.find("OK")){
        connectWifi();
    }
    interruptSetup();
}

void loop(){
    start:
        error = 0;
    delay(1000);
    updatebeat();
    if (error == 1){
        goto start;
    }
    delay(1000);
}

void updatebeat(){
    String cmd = "AT+CIPSTART=\"TCP\", \"";
    CMD += IP;
    cmd += "\",80";
    Serial.println(cmd);
    esp8266.println(cmd);
    delay(2000);
    if (esp8266.find("Error")){
        return;
    }
    cmd += msg;
    cmd += "&field1=";
    cmd += BPM;
    cmd += "\r\n";
    Serial.print("AT+CIPSEND=");
    esp8266.print("AT+CISEND=");
    Serial.println(cmd.length());
    esp8266.println(cmd.length());
    if(esp8266.find(">")) {
        Serial.print(cmd);
        esp8266.print(cmd);
    }
    else{
        Serial.println("AT+CIPCLOSE");
        esp8266.println("AT+CIPCLOSE");
        error = 1;
    }
}

void interruptSetup()}{
    TCCR2A = 0x02;
    TCCR2B = 0x06;
    OCR2A = 0X7C;
    TIMSK2 = 0x02;
    sei();
}

boolean connectWifi(){
    Serial.println("AT+CWMODE=1");
    esp8266.println("ACWMODE=1");
    delay(2000);
    String cmd = "AT+CWJAP=\"";
    cmd += SSID;
    cmd += "\", \"";
    cmd += PASS;
    cmd += "\"";
    Serial.println(cmd);
    esp8266.println(cmd);
    delay(5000);
    if(esp8266.find("OK")){
        Serial.println("OK");
        return true;
    }
    else {
        return false;
    }
}

ISR(TIMER2_COMPA_vect) {
  cli();
  Signal = analogRead(pulsePin);
  sampleCounter += 2;
  int N = sampleCounter - lastBeatTime;

  if (Signal < thresh && N > (IBI / 5) * 3) {
    if (Signal < T) {
      T = Signal;
    }
  }

  if (Signal > thresh && Signal > P) {
    P = Signal;
  }

  if (N > 250) {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3)) {
      Pulse = true;
      digitalWrite(blinkPin, HIGH);
      IBI = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;

      if (secondBeat) {
        secondBeat = false;
        for (int i = 0; i <= 9; i++) {
          rate[i] = IBI;
        }
      }

      if (firstBeat) {
        firstBeat = false;
        secondBeat = true;
        sei();
        return;
      }
      word runningTotal = 0;

      for (int i = 0; i <= 8; i++) {
        rate[i] = rate[i + 1];
        runningTotal += rate[i];
      }

      rate[9] = IBI;
      runningTotal += rate[9];
      runningTotal /= 10;
      BPM = 60000 / runningTotal;
      QS = true;
    }
  }

  if (Signal < thresh && Pulse == true) {
    digitalWrite(blinkPin, LOW);
    Pulse = false;
    amp = P - T;
    thresh = amp / 2 + T;
    P = thresh;
    T = thresh;
  }

  if (N > 2500) {
    thresh = 512;
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;
    firstBeat = true;
    secondBeat = false;
  }
  sei();
}