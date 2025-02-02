// Motor A pin connections
int enA = 9;
int in1 = 8;
int in2 = 7;
const int motorTime = 20000;
bool heliDetected = false;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Turn off motor - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Start Serial Communication
  Serial.begin(9600);
}

void loop() {
  // Check if there's data available in Serial
  if (Serial.available() > 0) {
    char received = Serial.read(); // Read one byte

    if (received == '1') {
      heliDetected = true;
      Serial.println("Helipad detected - Ready to activate motor");
    } else if (received == '0') {
      heliDetected = false;
      Serial.println("No helipad - Motor will not activate");
    }
  }

  // If helipad detected, activate motor
  if (heliDetected) {
    Serial.println("Activating motor...");
    analogWrite(enA, 255);   // Set motor speed to max
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    delay(motorTime); // Run motor for 20 seconds
    
    // Stop motor
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    Serial.println("Motor stopped");

    // Reset heliDetected to prevent re-triggering until new detection occurs
    heliDetected = false;
  }
}