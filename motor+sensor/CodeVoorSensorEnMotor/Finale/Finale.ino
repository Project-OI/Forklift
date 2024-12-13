const int DIR_PIN = 21;       // Richtingspin motor 1
const int STEP_PIN = 19;      // Stappenpin motor 1
const int DIR2_PIN = 17;      // Richtingspin motor 2
const int STEP2_PIN = 16;     // Stappenpin motor 2

const int BUTTON_PIN = 15;    // Knop op pin 15 (start motoren)
const int HALL_SENSOR_PIN = 2; // Hall sensor op pin 2
const int BUTTON_23_PIN = 23;  // Knop op pin 23 voor omhoog bewegen
const int BUTTON_18_PIN = 18;  // Knop op pin 18 voor autorisatie van knop 23
const int HALL_SENSOR_22_PIN = 22; // Tweede Hall sensor op pin 22
const int BUTTON_4_PIN = 4; //knop pin 4
const int MOTOR_SPEED = 750;  // Snelheid van de motoren in microseconden

bool klik = false;    // Variabele om te controleren of knop 23 mag werken

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);     // Knop op pin 15
  pinMode(HALL_SENSOR_PIN, INPUT);       // Hall sensor op pin 2
  pinMode(BUTTON_23_PIN, INPUT_PULLUP);  // Knop op pin 23
  pinMode(BUTTON_18_PIN, INPUT_PULLUP);  // Knop op pin 18
  pinMode(HALL_SENSOR_22_PIN, INPUT);    // Hall sensor op pin 22
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);  // Knop op pin 18

  pinMode(DIR_PIN, OUTPUT);              // Motor 1 richting
  pinMode(STEP_PIN, OUTPUT);             // Motor 1 stappen
  pinMode(DIR2_PIN, OUTPUT);             // Motor 2 richting
  pinMode(STEP2_PIN, OUTPUT);            // Motor 2 stappen

  Serial.begin(115200);                  // Seriële communicatie voor serial monitor ig
}

void loop() {
  String serialInput = Serial.readString();
  serialInput.trim();
  // Controleer of de knop op pin 15 is ingedrukt
 // if (digitalRead(BUTTON_PIN) == LOW) {
    if (serialInput == "15"){
    Serial.println("Knop 15 ingedrukt");
    Motor(HALL_SENSOR_PIN, false); // Motoren draaien omlaag tot Hall sensor op pin 2 actief wordt
    delay(250);
    tijdMotor();
    delay(1000);
    stopMotor();
    delay(500);          // Debouncing delay 
    klik = true; // Zet autorisatie op true
  }

  // Controleer of de knop op pin 23 is ingedrukt en geautoriseerd is
 // if (digitalRead(BUTTON_23_PIN) == LOW && klik) {
   if (serialInput == "23"){
    Serial.println("Knop 23 ingedrukt en geautoriseerd");
    Motor(HALL_SENSOR_22_PIN, true); // Motoren draaien omhoog tot Hall sensor op pin 22 actief wordt
    klik = false; // Reset knopske autorisatie na uitvoering
    Serial.println("Autorisatie gereset");
  }
   //if (digitalRead(BUTTON_4_PIN) == LOW) {
   if (serialInput == "4"){
    tijd();
    delay(250);
    stopMotor();
    delay(250);          // Debouncing delay 
  }
}

void Motor(int hallSensorPin, bool omhoog) {
  digitalWrite(DIR_PIN, omhoog ? HIGH : LOW);
  digitalWrite(DIR2_PIN, omhoog ? HIGH : LOW);

  while (digitalRead(hallSensorPin) == LOW) { // Draai tot de Hall sensor hoog is
    stepMotor();
  }

  Serial.println("Hall sensor actief");

  delay(150);
  Serial.println("Motor beweegt kort omhoog uit bereik van de sensor");
  digitalWrite(DIR_PIN, HIGH); // Richting omhoog
  digitalWrite(DIR2_PIN, HIGH);

  for (int i = 0; i < 200; i++) { // Maak een paar stappen omhoog 

    stepMotor();
  }

  stopMotor();
}

void tijd() {
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(DIR2_PIN, LOW); 
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);
  for (int i = 0; i < 1000; i++) {  // Draai de motoren omlaag voor doosje afzetten, maak [1000] groter om meer naar beneden te gaan
    stepMotor();  // Maak een stap
  }
}

void tijdMotor() {
  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(DIR2_PIN, HIGH); 
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);
  for (int i = 0; i < 10000; i++) {  // Draai de motoren voor 15 sec, legt 10cm af indien < 10000, op <20000 legt ie 20cm af in 15sec
    stepMotor();  // Maak een stap
  }
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