
/* ###################### Ultrasonic sensor ########################## */
const int trigPin = 7;
const int echoPin = 6;
// Motor A pin connections
int enA = 5;
int in1 = 9;
int in2 = 10;
// spin motor for motorTime milliseconds
const int motorTime = 10000;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // Set ultrasonic sensor pins to I/O
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	
	// Turn off motor - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  if (inches <= 20){
    Serial.println("Time to Deploy");
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
  }

  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
