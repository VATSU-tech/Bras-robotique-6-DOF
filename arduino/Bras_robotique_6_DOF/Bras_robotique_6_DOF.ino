#include <Servo.h>
    
    Servo servo[6];
    
    #define joyX A0         // Axe X du joystick
    #define joyY A1        // Axe Y du joystick
    #define joyBtn 7      //Bouton poussoir du joystick

    #define joyUp 55
    #define joyDown 45
    #define zero 0
    #define maxSensor 100
    #define maxDegMotor 180
    #define btnDelay 250
    #define minMotorSpeed 90
    #define maxMotorSpeed 10
    #define sensorConvertVal 100
    #define baudeRate 115200
    
    int i = 0;
    int servos[] = { 3, 5, 6, 9, 10, 11 };
    int servNbr = (sizeof(servos) / sizeof(servos[0])) - 1;
    
    int joyValX;
    int joyValY;
    int time;
    int valeur[] = { 90, 60, 45, 135, 0, 90};
    
    unsigned long present_time = 0;
    unsigned long previous_time = 0;
    
    int joyBtnState = false;
    unsigned long previous_time_joyBtn = 0;
    
    void setup() {
      Serial.begin(baudeRate);
      pinMode(joyBtn, INPUT_PULLUP);

      for(int i= zero; i < (sizeof(servo)/sizeof(servo[zero])); i++){
        servo[i].attach(servos[i]);
        servo[i].write(valeur[i]);
      }
    }
    
    void loop() {
      present_time = millis();
    
      joyValX = analogRead(joyX);
      joyValY = analogRead(joyY);
      joyValX = map(joyValX, zero, maxSensor, zero, sensorConvertVal);
      joyValY = map(joyValY, zero, maxSensor, zero, sensorConvertVal);

      if(joyValX >= joyUp) time = map(joyValX,joyUp,sensorConvertVal,minMotorSpeed,maxMotorSpeed);
      else if(joyValX <= joyDown) time = map(joyValX,joyDown,zero,minMotorSpeed,maxMotorSpeed);
    
      if ((present_time - previous_time) >= time) {
        if (joyValX >= joyUp && valeur[i] < maxDegMotor) valeur[i]++;
        else if (joyValX <= joyDown && valeur[i] > zero) valeur[i]--;
        previous_time = present_time;
      }

      if ((present_time - previous_time_joyBtn) >= btnDelay) {
        bool changed = false;

        if (digitalRead(joyBtn) == LOW && joyValY > joyUp) {
          i++;
          if (i > servNbr) i = zero;
          changed = true;
          previous_time_joyBtn = present_time;
        }
        else if (digitalRead(joyBtn) == LOW && joyValY < joyDown) {
          i--;
          if (i < zero) i = servNbr;
          changed = true;
          previous_time_joyBtn = present_time;
        }

        if (changed) {
          Serial.print("Servo actif : ");
          Serial.println(i);
        }
      }

      joyBtnState = digitalRead(joyBtn);

      servo[i].write(valeur[i]);
    }