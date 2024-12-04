// Pinnen voor motoren en knoppen
const int DIR_PIN = 21;       // Richtingspin motor 1
const int STEP_PIN = 19;      // Stappenpin motor 1
const int DIR2_PIN = 17;      // Richtingspin motor 2
const int STEP2_PIN = 16;     // Stappenpin motor 2

const int BUTTON_PIN = 15;    // Knop op pin 15 (start motoren)
const int HALL_SENSOR_PIN = 2; // Hall sensor op pin 2
const int BUTTON_23_PIN = 23;  // Knop op pin 23 voor omhoog bewegen
const int HALL_SENSOR_22_PIN = 22; // Tweede Hall sensor op pin 22
const int BUTTON_18_PIN = 18; // Knop op pin 18 (voor toegang tot knop 23)

const int MOTOR_SPEED = 750;  // Snelheid van de motoren in microseconden

bool klik = false;    // Boolean om te controleren of pin 23 knop kan worden ingedrukt

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);     // Knop op pin 15
  pinMode(HALL_SENSOR_PIN, INPUT);       // Hall sensor op pin 2
  pinMode(BUTTON_23_PIN, INPUT_PULLUP);  // Knop op pin 23
  pinMode(HALL_SENSOR_22_PIN, INPUT);    // Hall sensor op pin 22
  pinMode(BUTTON_18_PIN, INPUT_PULLUP);  // Knop op pin 18 voor toegang tot knop 23

  pinMode(DIR_PIN, OUTPUT);              // Motor 1 richting
  pinMode(STEP_PIN, OUTPUT);             // Motor 1 stappen
  pinMode(DIR2_PIN, OUTPUT);             // Motor 2 richting
  pinMode(STEP2_PIN, OUTPUT);            // Motor 2 stappen

  Serial.begin(115200);                  // Seriële communicatie voor debugging
}

void loop() {
  // Controleer of de knop op pin 18 is ingedrukt (om toegang te krijgen tot pin 23)
  if (digitalRead(BUTTON_18_PIN) == LOW) {
    Serial.println("Knop 18 ingedrukt, pin 23 is nu beschikbaar om uitgevoerd te worden");
    klik = true;  // Zet de boolean op true zodat pin 23 knop werkt en de code daar kan worden uitgevoerd!
    delay(500); // Delayyy
  }

  // Als de knop op pin 15 wordt ingedrukt, start de motoren
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Knop 15 ingedrukt");
    digitalWrite(DIR_PIN, LOW);  // Zet richting naar omlaag
    digitalWrite(DIR2_PIN, LOW);
    runMotorUntilSensor(HALL_SENSOR_PIN); // Motoren draaien totdat Hall sensor op pin 2 actief wordt
  } else {
    Serial.println("motoren gaan uit");
    stopMotor();
  }

  // Als de knop op pin 23 wordt ingedrukt, en pin 18 is eerder ingedrukt, draai de motor omhoog
  if (digitalRead(BUTTON_23_PIN) == LOW && klik) {
    Serial.println("Knop 23 ingedrukt");
    digitalWrite(DIR_PIN, HIGH);  // Zet richting omhoog
    digitalWrite(DIR2_PIN, HIGH);
    runMotorUntilSensor(HALL_SENSOR_22_PIN); // Motoren draaien totdat Hall sensor op pin 22 actief wordt
    klik = false;
    delay(500);
  }
}

// Functie om motoren te laten draaien totdat een Hall sensor hoog is
void runMotorUntilSensor(int hallSensorPin) {
  while (digitalRead(hallSensorPin) == LOW) { // Draai de motoren totdat de Hall sensor actief is
    stepMotor();
  }

  Serial.println("Hall sensor actief");
  stopMotor();
}

// Functie om de motoren te stoppen
void stopMotor() {
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);
}

// Stappenfunctie voor motoren
void stepMotor() {
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP2_PIN, HIGH);
  delayMicroseconds(MOTOR_SPEED);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);
  delayMicroseconds(MOTOR_SPEED);
}
