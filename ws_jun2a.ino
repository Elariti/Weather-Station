#include <SoftwareSerial.h>
#include <LiquidCrystal.h> 
#include "dht.h"

// Pin and sensor definitions
#define dataPin A1
#define TILT_SENSOR_BIT 7  

// LCD and sensor object initializations
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
dht DHT;
SoftwareSerial bt(9, 10); 

// Global variables for sensor readings
int temp;
int hum;
float dewPoint;
volatile bool earthquakeDetected = false; 

/*--
********************************************************
-- Purpose: Initialize system components and settings
-- Sets up serial communication, LCD, and sensor interrupt
******************************************************** --*/
void setup() {
  // Initialize communication interfaces
  Serial.begin(9600); 
  bt.begin(9600); 
  Serial.println("Ready");
  
  // Display welcome message on LCD
  lcd.begin(16, 2);  
  lcd.setCursor(0, 0);
  lcd.print("WELCOME To My ");
  lcd.setCursor(0, 1);
  lcd.print("YouTube Channel");
  delay(2000);
  lcd.clear(); 

  // Configure tilt sensor with interrupt
  pinMode(TILT_SENSOR_BIT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TILT_SENSOR_BIT), earthquakeISR, FALLING);
}

/*--
********************************************************
-- Purpose: Main program loop for sensor monitoring
-- Reads sensor data, displays on LCD, and transmits via Bluetooth
******************************************************** --*/
void loop() {
  // Read temperature and humidity from DHT11 sensor
  int readData = DHT.read11(dataPin);

  hum = DHT.humidity;
  temp = DHT.temperature;

  // Calculate dew point
  dewPoint = temp - ((100 - hum) / 5.0);

  // Display humidity on LCD
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.print("% ");

  // Display temperature on LCD
  lcd.setCursor(0, 1); 
  lcd.print("Temp: "); 
  lcd.print(temp); 
  lcd.print((char)223); 
  lcd.print("C ");

  // Transmit data via Bluetooth
  bt.print(temp); 
  bt.print(";"); 
  bt.print(hum); 
  bt.print(";"); 
  bt.print(dewPoint); 
  bt.println(";");

  // Print data to Serial monitor
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("; Humidity: ");
  Serial.print(hum);
  Serial.print("; Dew Point: ");
  Serial.println(dewPoint);

  // Check for earthquake detection
  if (earthquakeDetected) {
    showEarthquakeWarning();
    earthquakeDetected = false; 
  }

  delay(1000);
}

/*--
********************************************************
-- Purpose: Interrupt Service Routine for earthquake detection
-- Sets global flag when tilt is detected
******************************************************** --*/
void earthquakeISR() {
  earthquakeDetected = true;
}

/*--
********************************************************
-- Purpose: Display earthquake warning
-- Shows message on LCD and transmits warning via Bluetooth and Serial
******************************************************** --*/
void showEarthquakeWarning() {
  // Clear LCD and display warning
  lcd.clear();
  lcd.print("Earthquake");
  lcd.setCursor(0, 1);
  lcd.print("Warning!");

  // Send warning via Bluetooth and Serial
  bt.println("Earthquake Warning!");
  Serial.println("Earthquake Detected!");

  delay(3000); 
  lcd.clear();
}

