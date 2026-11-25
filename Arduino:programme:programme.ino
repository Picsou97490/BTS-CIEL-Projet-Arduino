#include <Ultrasonic.h>

// Configuration des broches
const int LED = 13;
Ultrasonic ultrasonic(7);

bool etatLED = false;
unsigned long derniereMesure = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Communication USB avec Python
  Serial.begin(9600);

  // Communication Bluetooth HC-05
  Serial1.begin(9600);
}

void commande(char c) {
  // Allumer la LED
  if (c == '1' || c == 'A') {
    etatLED = true;
  }
  // Eteindre la LED
  else if (c == '0' || c == 'E') {
    etatLED = false;
  }
  else {
    return;
  }

  digitalWrite(LED, etatLED ? HIGH : LOW);

  // Confirmation vers Python
  Serial.println(etatLED ? "LED:ON" : "LED:OFF");

  // Confirmation vers Bluetooth
  Serial1.println(etatLED ? "A" : "E");
}

void loop() {
  // Reception des commandes USB
  while (Serial.available()) {
    commande(Serial.read());
  }

  // Reception des commandes Bluetooth
  while (Serial1.available()) {
    commande(Serial1.read());
  }

  // Mesure toutes les 500 ms
  if (millis() - derniereMesure >= 500) {
    derniereMesure = millis();

    long distance = ultrasonic.MeasureInCentimeters();

    // Envoi vers Python
    Serial.print("DIST:");
    Serial.println(distance);

    // Envoi vers Android sur 3 chiffres
    if (distance >= 0 && distance <= 999) {
      if (distance < 100) Serial1.print("0");
      if (distance < 10) Serial1.print("0");
      Serial1.println(distance);
    }
  }
}
