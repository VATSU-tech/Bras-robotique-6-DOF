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
    
    // Variables pour mémoriser l'état précédent des boutons (détection de clic)
    int lastBtnState = LOW;
    int lastBtn2State = LOW;
    unsigned long lastDebounceTime = 0;
    
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
    
      // Initialisation des servos à 90°
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
    
      // 1. Contrôle par Joystick (incrément / décrément continu)
      val = analogRead(potpin);
      val = map(val, 0, 1023, 0, 100);
    

Resume with -c (or command below):
agy --conversation=618d363c-f2e8-4d36-a0b4-25e34321b688

      if ((present_time - previous_time) >= 10) {
        if (val >= 75 && valeur[0] < 180) {
          valeur[0]++;
        } else if (val <= 25 && valeur[0] > 0) {
          valeur[0]--;
        }
        previous_time = present_time;
      }

      // 2. Détection précise des clics sur les boutons (avec résistances pull-down)
      int readingBtn = digitalRead(btn);
      int readingBtn2 = digitalRead(btn2);

      if ((present_time - lastDebounceTime) >= 150) {
        bool changed = false;

        // Détection d'un appui sur btn (servo suivant)
        if (readingBtn == HIGH && lastBtnState == LOW) {
          i++;
          if (i > 6) i = 1;
          changed = true;
          lastDebounceTime = present_time;
        }
        // Détection d'un appui sur btn2 (servo précédent)
        else if (readingBtn2 == HIGH && lastBtn2State == LOW) {
          i--;
          if (i < 1) i = 6;
          changed = true;
          lastDebounceTime = present_time;
        }

        // Si on a changé de servo, on recharge sa position actuelle
        if (changed) {
          valeur[0] = valeur[i];
          Serial.print("Servo actif : ");
          Serial.println(i);
        }
      }

      lastBtnState = readingBtn;
      lastBtn2State = readingBtn2;

      // 3. Mise à jour de la position du servo actif
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