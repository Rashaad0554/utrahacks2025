// Motor A pin connections
int enA = 9;
int in1 = 8;
int in2 = 7;
const int motorTime = 2000;
bool heliDetected = false;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Turn off motor - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  // Start Serial Communication
  Serial.begin(9600);
}

void loop() {
  // Check if there's data available in Serial
  if (Serial.available() > 0) {
    char received = Serial.read(); // Read one byte

    if (received == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Helipad detected - Ready to activate motor");
      // Set motor A speed to max
      analogWrite(enA, 255);
      // Turn on motor
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // turn motor for 20 seconds
      delay(motorTime);
      // Turn off motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    } else if (received == '0') {
      Serial.println("No helipad - Motor will not activate");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
