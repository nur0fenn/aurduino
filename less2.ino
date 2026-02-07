#include <Servo.h>
      
#define POT_PIN_1 A3
#define POT_PIN_2 A4
#define POT_PIN_3 A1
#define POT_PIN_4 A5

#define SERVO_PIN_1 2
#define SERVO_PIN_2 3
#define SERVO_PIN_3 4
#define SERVO_PIN_4 5

const int servo9g1_1RestPosition = 20;
const int servo9g2_2RestPosition = 20;
const int servo9g3_3RestPosition = 20;
const int servo9g4_4RestPosition = 20;

int val;

Servo servo9g1_1;
Servo servo9g2_2;
Servo servo9g3_3;
Servo servo9g4_4;

void setup(){
    servo9g1_1.attach(SERVO_PIN_1);
    servo9g1_1.write(servo9g1_1RestPosition);

    servo9g2_2.attach(SERVO_PIN_2);
    servo9g2_2.write(servo9g2_2RestPosition);

    servo9g3_3.attach(SERVO_PIN_3);
    servo9g3_3.write(servo9g3_3RestPosition);

    servo9g4_4.attach(SERVO_PIN_4);
    servo9g4_4.write(servo9g4_4RestPosition); 

    delay(100);
}

void loop(){
    val = analogRead(POT_PIN_1);
    val = map(val, 0, 1023, 0, 179);
    servo9g1_1(val);
    delay(15);

    val = analogRead(POT_PIN_2);
    val = map(val, 0, 1023, 0, 179);
    servo9g2_2(val);
    delay(15);

    val = analogRead(POT_PIN_3);
    val = map(val, 0, 1023, 0, 179);
    servo9g3_3(val);
    delay(15);

    val = analogRead(POT_PIN_4);
    val = map(val, 0, 1023, 0, 179);
    servo9g4_4(val);
    delay(15);
}