// Pinnen voor motoren en knoppen
const int DIR_PIN = 21;       // Richtingspin motor 1
const int STEP_PIN = 19;      // Stappenpin motor 1
const int DIR2_PIN = 17;      // Richtingspin motor 2
const int STEP2_PIN = 16;     // Stappenpin motor 2

const int BUTTON_PIN = 15;    // Knop op pin 15 (start motoren)
const int HALL_SENSOR_PIN = 2; // Hall sensor op pin 2
const int BUTTON_23_PIN = 23;  // Knop op pin 23 voor omhoog bewegen
const int HALL_SENSOR_22_PIN = 22; // Tweede Hall sensor op pin 22
const int MOTOR_SPEED = 750;  // Snelheid van de motoren in microseconden

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);     // Knop op pin 15
  pinMode(HALL_SENSOR_PIN, INPUT);       // Hall sensor op pin 2
  pinMode(BUTTON_23_PIN, INPUT_PULLUP);  // Knop op pin 23
  pinMode(HALL_SENSOR_22_PIN, INPUT);    // Hall sensor op pin 22

  pinMode(DIR_PIN, OUTPUT);              // Motor 1 richting
  pinMode(STEP_PIN, OUTPUT);             // Motor 1 stappen
  pinMode(DIR2_PIN, OUTPUT);             // Motor 2 richting
  pinMode(STEP2_PIN, OUTPUT);            // Motor 2 stappen

  Serial.begin(115200);                  // Seriële communicatie voor debugging
}

void loop() {
  // Controleer of de knop op pin 15 is ingedrukt
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Knop 15 ingedrukt");
    runMotorUntilSensor(HALL_SENSOR_PIN, false); // Motoren draaien omlaag tot Hall sensor op pin 2 actief wordt
  }

  // Controleer of de knop op pin 23 is ingedrukt
  if (digitalRead(BUTTON_23_PIN) == LOW) {
    Serial.println("Knop 23 ingedrukt");
    runMotorUntilSensor(HALL_SENSOR_22_PIN, true); // Motoren draaien omhoog tot Hall sensor op pin 22 actief wordt
  }
}

// Functie om motoren te laten draaien tot een Hall sensor actief wordt
void runMotorUntilSensor(int hallSensorPin, bool omhoog) {
  // Stel de richting in afhankelijk van omhoog/omlaag
  digitalWrite(DIR_PIN, omhoog ? HIGH : LOW);
  digitalWrite(DIR2_PIN, omhoog ? HIGH : LOW);

  while (digitalRead(hallSensorPin) == LOW) { // Draai tot de Hall sensor hoog is
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
