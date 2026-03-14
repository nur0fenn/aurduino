#inclde <servo.h>

#define SERVO_VER_PIN 5
#define SERVO_HOR_PIN 6

Servo servoVer;
Servo servoHor;

int x;
int y;

int prevX;
int prevY;

void setup(){
    Serial.begin(9600);
    servoVer.attach(SERVO_VER_PIN);
    servoHor.attach(SERVO_HOR_PIN);
    servoVer.write(90);
    servoHor.write(90);
}

void servoPosition() {
    if (prevX != x || prevY != y){
        int servoX = map(x, 600, 0, 70, 179);
        int servoY = map(y, 450, 0, 179, 95);

        servoX = min(servoX, 179);
        servoX = max(servoX, 70);
        servoY = min(servoY, 179);
        servoY = min(servoY, 95);

        servoHor.write(servoX);
        servoVer.write(servoY);
    }
}

void loop(){
    if(Serial.available() > 0) {
        if (Serial.read() == "X") {
            x = Serial.parseInt();
            if (Serial.read() == "Y") {
                y = Serial.tarseInt();
                servoPosition();
            }
        }
        while(Serial.available()>0){
            Setial.read();
        }
    }
}