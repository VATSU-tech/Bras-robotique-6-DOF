#include <Servo.h>
    
    Servo servo[5];
    
    #define btn 2
    #define btn2 4
    
    int i = 1;
    int servos[] = { 3, 5, 9, 10, 11 };
    
    int val;
    int time;
    int potpin = A0;  // Axe du joystick
    int valeur[7] = { 90, 90, 90, 90, 90, 90, 90 };
    
    unsigned long present_time = 0;
    unsigned long previous_time = 0;
    
    int btnState = false;
    int btnState2 = false;
    unsigned long previous_time_btn = 0;
    
    void setup() {
      Serial.begin(115200);
      pinMode(btn, INPUT);
      pinMode(btn2, INPUT);

      for(int i= 0; i < 5; i++){
        servo[i].attach(servos[i]);
        servo[i].write(valeur[i+1]);
      }
    
      valeur[0] = valeur[i];
    }
    
    void loop() {
      present_time = millis();
    
      val = analogRead(potpin);
      val = map(val, 0, 1023, 0, 100);

      if(val >= 55) time = map(val,55,100,90,10);
      else if(val <= 45) time = map(val,45,0,90,10);
    
      if ((present_time - previous_time) >= time) {
        if (val >= 55 && valeur[0] < 180) valeur[0]++;
        else if (val <= 45 && valeur[0] > 0) valeur[0]--;
        previous_time = present_time;
      }

      if ((present_time - previous_time_btn) >= 150) {
        bool changed = false;

        if (digitalRead(btn) == HIGH && btnState == LOW) {
          i++;
          if (i > 5) i = 1;
          changed = true;
          previous_time_btn = present_time;
        }
        else if (digitalRead(btn2) == HIGH && btnState2 == LOW) {
          i--;
          if (i < 1) i = 5;
          changed = true;
          previous_time_btn = present_time;
        }

        if (changed) {
          valeur[0] = valeur[i];
          Serial.print("Servo actif : ");
          Serial.println(i);
        }
      }

      btnState = digitalRead(btn);
      btnState2 = digitalRead(btn2);

      valeur[i] = valeur[0];
      Serial.print(val);
      Serial.print(" ");
      Serial.print(time);
      Serial.print(" ");
      Serial.println(valeur[i]);

      servo[i-1].write(valeur[i]);
    }