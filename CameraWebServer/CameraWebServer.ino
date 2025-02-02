void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set up an output pin (e.g., for an LED)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Ensure the LED is off initially
}

void loop() {
  // Check if data is available from the Python script
  if (Serial.available() > 0) {
    char signal = Serial.read();  // Read the incoming signal

    // If the signal is '1', turn on the LED
    if (signal == '1') {
      digitalWrite(LED_BUILTIN, HIGH);  // Enable signal
      Serial.println("Received '1'. LED ON.");
    }
    // If the signal is '0', turn off the LED
    else if (signal == '0') {
      digitalWrite(LED_BUILTIN, LOW);  // Disable signal
      Serial.println("Received '0'. LED OFF.");
    }
  }
}