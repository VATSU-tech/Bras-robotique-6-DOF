/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo
#define btn 2
#define btn2 4
int i= 0;
bool activate = false;

int servos[] = {3,5,6,9,10,11};

int potpin = A0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int valeur = 90;
unsigned long present_time = 0;
unsigned long previous_time =0;

void setup() {
  Serial.begin(115200);
  pinMode(btn, INPUT);
  pinMode(btn2, INPUT);
  myservo.attach(servos[0]);  // attaches the servo on pin 9 to the Servo object
}

void loop() {
  present_time = millis();
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 100);     // scale it for use with the servo (value between 0 and 180)
  if(valeur > 180) valeur = 180;
  else if(valeur < 0) valeur = 0;


  if((present_time - previous_time)>= 10){
      if(val >= 75) valeur ++;
      else if(val <= 25) valeur --;
    previous_time = present_time;
  }

  if(activate){
    if(i>=6 || i < 0)i=0;
    myservo.detach();
    myservo.attach(servos[i]);
    activate = false;
  }
  if(digitalRead(btn))i++;
  if(digitalRead(btn2))i--;
  if(digitalRead(btn) || digitalRead(btn2)){
    activate = true;
    delay(250);
  }
  Serial.println(i);

  myservo.write(valeur);                  // sets the servo position according to the scaled value
  // delay(15);                           // waits for the servo to get there
}
