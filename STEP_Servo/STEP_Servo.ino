#include <Servo.h>
// A4988 driver test routine
#define ENABLE A2
#define STEP A6
#define DIR A7

#define STEPS_PER_ROTATION 3000
int FLAG_Home = 0;
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int current_pos = 0;//当前位置
char muisc[12] = {1, 1, 5, 5, 6, 6, 5, 4, 4, 3, 3, 3};
void setup() {
  Serial.begin(57600);
  myservo.attach(45);  // attaches the servo on pin 9 to the servo object
  pinMode(ENABLE, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, Start, FALLING);    //设置触发，中断号0，即数字2口
}

void hit()
{
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }

}
void Start()
{
  FLAG_Home = 1;
  Serial.println("*******");


}

void stepNow(int totalSteps) {
  Serial.print(totalSteps);
  Serial.println(F(" steps."));

  int i;
  for (i = 0; i < totalSteps; ++i) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP, LOW);
    delayMicroseconds(50);
  }
}


void walkBothDirections() {
  Serial.println(F("dir LOW"));
  digitalWrite(DIR, HIGH);
  stepNow(STEPS_PER_ROTATION);

  Serial.println(F("dir HIGH"));
  digitalWrite(DIR, LOW);
  stepNow(STEPS_PER_ROTATION);
}

void left(int number)  //左移动
{
  digitalWrite(DIR, LOW);
  stepNow(1000 * number);

}
void right(int number)  //向右移动
{
  digitalWrite(DIR, HIGH);
  stepNow(1000 * number);

}
void movement(int pos_next, int pos_present)  //通过音符直接移动
{
  if (pos_next > pos_present)
    left(pos_next - pos_present);
  else
    right(pos_present - pos_next);
  pos_present = pos_next;
}
void movement_hit(int pos_next, int pos_present)
{
  if (pos_next > pos_present)
    left(pos_next - pos_present);
  else
    right(pos_present - pos_next);
  pos_present = pos_next;
  hit();
}

void loop() {
  //  Serial.println(F("Enable HIGH"));
  //  digitalWrite(ENABLE,HIGH);
  //  walkBothDirections();
  //  Serial.println(F("Enable LOW"));
  //  digitalWrite(ENABLE,LOW);
  //  walkBothDirections();
  if (FLAG_Home == 0)
  {
    digitalWrite(DIR, HIGH);
    stepNow(10);
  }
  else
  {
    for (int i = 0; i < 12; i++)
    {
      movement_hit(music[i],0);
    }
    left(1);
    hit();
    left(4);
    hit();
    left(1);
    hit();
    right(1);
    right(1);
    hit();
    right(1);
    hit();
    right(1);
    hit();
    FLAG_Home = 0;
  }

}
