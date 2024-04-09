#include <DHT.h> // Include the library for the DHT sensor

const int DHTPIN = 5; // DHT11 Sensor is on digital pin 5
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

const int motionSensorPin = 3; // PIR Motion Sensor is on digital pin 3
const int buttonPin = 4; // Push button is on digital pin 4

int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

unsigned long lastTempHumidityUpdate = 0; // Last time temperature and humidity were updated
const long tempHumidityUpdateInterval = 5000; // Update interval for temperature and humidity

unsigned long lastMotionDetected = 0; // Track the last time motion was detected
const long motionDetectionInterval = 6000; // Interval before the next motion can be detected (10 seconds)


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(motionSensorPin, INPUT); // Initialize the motion sensor pin as an input
  Serial.begin(9600); // Begin Serial communication
  dht.begin(); // Initialize the DHT sensor
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle Motion Detection
  handleMotion(currentMillis);

  // Handle Temperature and Humidity Readings
  handleTempHumidityReadings(currentMillis);

  // Handle Button Press
  handleButtonPress();
}

void handleMotion(unsigned long currentMillis) {
  if (currentMillis - lastMotionDetected > motionDetectionInterval) {
    byte motionState = digitalRead(motionSensorPin);
    if (motionState == HIGH) {
      // Motion detected; perform an action if needed
      Serial.println("Motion Detected");
      lastMotionDetected = currentMillis; // Update the last motion detected time
    }
  }
}

void handleTempHumidityReadings(unsigned long currentMillis) {
  if (currentMillis - lastTempHumidityUpdate >= tempHumidityUpdateInterval) {
    lastTempHumidityUpdate = currentMillis;
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (!isnan(humidity) && !isnan(temperature)) {
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.print("%  Temperature: ");
      Serial.print(temperature);
      Serial.println("C");
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  }
}

void handleButtonPress() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      // Button is pressed; perform an action if needed
      Serial.println("Button Pressed");
    }
    lastButtonState = buttonState; // Save the reading for the next loop iteration
  }
}
