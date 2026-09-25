#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define ADRESSE_OLED 0x3C
#define LARGEUR 128
#define HAUTEUR 64

Servo servo[6];
Adafruit_SH1106G display(LARGEUR, HAUTEUR, &Wire, -1);

// Pins
#define joyX A0
#define joyY A1
#define joyBtn 7

// Config
#define joyUp 55
#define joyDown 45
#define maxSensor 1023
#define maxDegMotor 180
#define btnDelay 250
#define minMotorSpeed 90 // le plus lent
#define maxMotorSpeed 10 // le plus rapide
#define sensorConvertVal 100

int servos[] = { 3, 5, 6, 9, 10, 11 };
const int servNbr = sizeof(servos) / sizeof(servos[0]); // 6
const char* nomsServos[] = {"Base", "Epaule", "Coude", "Poignet", "Pince", "Rot"};

int servoActif = 0;
int joyValX, joyValY;
int delaiMoteur = 1000; // remplace ton 'time' - initialise a 1s au repos
int valeur[] = { 90, 60, 45, 135, 0, 90 };

unsigned long present_time = 0;
unsigned long previous_time = 0;
unsigned long previous_time_screen = 0;
unsigned long previous_time_joyBtn = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(joyBtn, INPUT_PULLUP);

  if (!display.begin(ADRESSE_OLED, true)) {
    Serial.println("OLED non detecte! Verifie A4/A5");
    while (1);
  }

  for (int k = 0; k < servNbr; k++) {
    servo[k].attach(servos[k]);
    servo[k].write(valeur[k]);
  }

  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 0);
  display.println("OLED 1.3 I2C 128x64");
  display.setCursor(50, 20);
  display.println("SH1106");
  display.setTextSize(2);
  display.setCursor(30, 40);
  display.println("Karibu");
  display.display();
  delay(2000);
}

void loop() {
  present_time = millis();

  joyValX = analogRead(joyX);
  joyValY = analogRead(joyY);
  joyValX = map(joyValX, 0, maxSensor, 0, sensorConvertVal);
  joyValY = map(joyValY, 0, maxSensor, 0, sensorConvertVal);

  // Calcul de la vitesse seulement si joystick bouge
  if (joyValX >= joyUp) {
    delaiMoteur = map(joyValX, joyUp, sensorConvertVal, minMotorSpeed, maxMotorSpeed);
  } else if (joyValX <= joyDown) {
    delaiMoteur = map(joyValX, joyDown, 0, minMotorSpeed, maxMotorSpeed);
  }

  // Deplacement du servo actif
  if ((present_time - previous_time) >= delaiMoteur) {
    if (joyValX >= joyUp && valeur[servoActif] < maxDegMotor) {
      valeur[servoActif]++;
    } else if (joyValX <= joyDown && valeur[servoActif] > 0) {
      valeur[servoActif]--;
    }
    servo[servoActif].write(valeur[servoActif]);
    previous_time = present_time;
  }

  // Changement de servo : Bouton appuye + Joystick Haut/Bas
  if ((present_time - previous_time_joyBtn) >= btnDelay) {
    if (digitalRead(joyBtn) == LOW) {
      if (joyValY > joyUp) {
        servoActif++;
        if (servoActif >= servNbr) servoActif = 0;
        Serial.print("Servo actif : "); Serial.println(servoActif);
        previous_time_joyBtn = present_time;
      } else if (joyValY < joyDown) {
        servoActif--;
        if (servoActif < 0) servoActif = servNbr - 1;
        Serial.print("Servo actif : "); Serial.println(servoActif);
        previous_time_joyBtn = present_time;
      }
    }
  }

  // Affichage OLED toutes les 100ms
  if ( false && (present_time - previous_time_screen) >= 100) {
    previous_time_screen = present_time; // <--- C'ETAIT LE BUG

    int speedPercent = 0;
    if (joyValX >= joyUp) speedPercent = map(joyValX, joyUp, 100, 0, 100);
    else if (joyValX <= joyDown) speedPercent = map(joyValX, joyDown, 0, 0, 100);

    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(13, 0);
    display.print(nomsServos[servoActif]);
    display.fillRect(0, 9, 60, 1, SH110X_WHITE);

    display.setCursor(0, 16);
    display.print("MOT");
    display.setTextSize(2);
    display.setCursor(20, 13);
    display.print(servoActif);
    display.fillRect(33, 10, 1, 22, SH110X_WHITE);

    display.setTextSize(1);
    display.setCursor(38, 12);
    display.print("Deg");
    display.setCursor(38, 22);
    display.print(valeur[servoActif]);

    display.setCursor(0, 35);
    display.print("joyX");
    display.setCursor(1, 46);
    display.print(joyValX);
    display.print("%");
    display.fillRect(25, 33, 1, 22, SH110X_WHITE);

    display.setCursor(30, 35);
    display.print("Speed");
    display.setCursor(31, 46);
    display.print(speedPercent);
    display.print("%");

    display.fillRect(60, 0, 1, 56, SH110X_WHITE);
    display.fillRect(0, 32, 60, 1, SH110X_WHITE);

    display.display();
  }
}