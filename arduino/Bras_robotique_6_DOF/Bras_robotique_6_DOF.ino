#include <Servo.h>
    
    Servo servo1, servo2, servo3, servo4, servo5, servo6;
    
    #define btn 2
    #define btn2 4
    
    int i = 1;
    int servos[] = { 3, 5, 6, 9, 10, 11 };
    
    int potpin = A0;  // Axe du joystick
    int val;
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
    
      servo1.attach(servos[0]);
      servo2.attach(servos[1]);
      servo3.attach(servos[2]);
      servo4.attach(servos[3]);
      servo5.attach(servos[4]);
      servo6.attach(servos[5]);
    
      servo1.write(valeur[1]);
      servo2.write(valeur[2]);
      servo3.write(valeur[3]);
      servo4.write(valeur[4]);
      servo5.write(valeur[5]);
      servo6.write(valeur[6]);
    
      valeur[0] = valeur[i];
    }
    
    void loop() {
      present_time = millis();
    
      val = analogRead(potpin);
      val = map(val, 0, 1023, 0, 100);
    
      if ((present_time - previous_time) >= 20) {
        if (val >= 75 && valeur[0] < 180) valeur[0]++;
        else if (val <= 25 && valeur[0] > 0) valeur[0]--;
        previous_time = present_time;
      }

      if ((present_time - previous_time_btn) >= 150) {
        bool changed = false;

        if (digitalRead(btn) == HIGH && btnState == LOW) {
          i++;
          if (i > 6) i = 1;
          changed = true;
          previous_time_btn = present_time;
        }
        else if (digitalRead(btn2) == HIGH && btnState2 == LOW) {
          i--;
          if (i < 1) i = 6;
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

      switch (i) {
        case 1: servo1.write(valeur[1]); break;
        case 2: servo2.write(valeur[2]); break;
        case 3: servo3.write(valeur[3]); break;
        case 4: servo4.write(valeur[4]); break;
        case 5: servo5.write(valeur[5]); break;
        case 6: servo6.write(valeur[6]); break;
      }
    }