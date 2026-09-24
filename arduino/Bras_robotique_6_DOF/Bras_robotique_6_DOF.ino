#include <Servo.h>
    
    Servo servo[6];
    
    #define joyX A0         // Axe X du joystick
    #define joyY A1        // Axe Y du joystick
    #define joyBtn 7      //Bouton poussoir du joystick
    
    int i = 1;
    int servos[] = { 3, 5, 6, 9, 10, 11 };
    
    int joyValX;
    int joyValY;
    int time;
    int valeur[] = { 90, 90, 90, 90, 90, 90, 90};
    
    unsigned long present_time = 0;
    unsigned long previous_time = 0;
    
    int joyBtnState = false;
    unsigned long previous_time_joyBtn = 0;
    
    void setup() {
      Serial.begin(115200);
      pinMode(joyBtn, INPUT_PULLUP);

      for(int i= 0; i < 6; i++){
        servo[i].attach(servos[i]);
        servo[i].write(valeur[i+1]);
      }
    
      valeur[0] = valeur[i];
    }
    
    void loop() {
      present_time = millis();
    
      joyValX = analogRead(joyX);
      joyValY = analogRead(joyY);
      joyValX = map(joyValX, 0, 1023, 0, 100);
      joyValY = map(joyValY, 0, 1023, 0, 100);

      if(joyValX >= 55) time = map(joyValX,55,100,90,10);
      else if(joyValX <= 45) time = map(joyValX,45,0,90,10);
    
      if ((present_time - previous_time) >= time) {
        if (joyValX >= 55 && valeur[0] < 180) valeur[0]++;
        else if (joyValX <= 45 && valeur[0] > 0) valeur[0]--;
        previous_time = present_time;
      }

      if ((present_time - previous_time_joyBtn) >= 250) {
        bool changed = false;

        if (digitalRead(joyBtn) == LOW && joyValY > 55) {
          i++;
          if (i > 6) i = 1;
          changed = true;
          previous_time_joyBtn = present_time;
        }
        else if (digitalRead(joyBtn) == LOW && joyValY < 45) {
          i--;
          if (i < 1) i = 6;
          changed = true;
          previous_time_joyBtn = present_time;
        }

        if (changed) {
          valeur[0] = valeur[i];
          Serial.print("Servo actif : ");
          Serial.println(i);
        }
      }

      joyBtnState = digitalRead(joyBtn);

      valeur[i] = valeur[0];
      Serial.print(joyValY);
      Serial.print(" ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(valeur[i]);

      servo[i-1].write(valeur[i]);
    }