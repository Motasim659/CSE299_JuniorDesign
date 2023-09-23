// Include required libraries
#include <Wire.h> // I2C communication protocol library
#include <LiquidCrystal_I2C.h> // LCD library
#include <DHT.h> // DHT11 temperature and humidity sensor library

#include<SoftwareSerial.h>
SoftwareSerial B(10,11);    //10-RX,  11-TX

// Define DHT11 sensor digital pin and type
#define DHTPIN 2
#define DHTTYPE DHT11

// Initialize DHT11 and LCD modules
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define analog input pins for sensors and digital output pins for fan and buzzer
const int soilPin = A0;
const int buzzerPin = 8;
const int fanPin = 9;
const int LDR_PIN = A5;

void setup() {

  B.begin(9600);

  Serial.begin(9600); // Start serial communication
  dht.begin(); // Start DHT11 sensor

  lcd.begin(); // Initialize LCD module
  lcd.backlight(); // Turn on LCD backlight

  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
  pinMode(fanPin, OUTPUT); // Set the fan pin as an output
  pinMode(LDR_PIN, INPUT); // Set the LDR pin as an input
}

void loop() {
  float h = dht.readHumidity(); // Read humidity value from DHT11
  float t = dht.readTemperature(); // Read temperature value from DHT11

  // Check if the DHT11 sensor reading is valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display temperature and humidity on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

  
  
  B.print("Temp: ");
  B.print(t);
  B.print("C");
  B.print("Humidity: ");
  B.print(h);
  B.print("%");

  // Print temperature and humidity values to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("% Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Turn on the fan if the temperature is over 30°C
  if (t > 30) {
    digitalWrite(fanPin, HIGH); // Turn on the fan
  } else {
    digitalWrite(fanPin, LOW); // Turn off the fan
  }

  delay(2000); // Display temperature and humidity for 2 seconds

  // Read soil moisture sensor value
  int soilMoisture = analogRead(soilPin);

  // Convert analog value to percentage
  int percentMoisture = map(soilMoisture, 0, 1023, 0, 100);

  // Display soil moisture and watering status on LCD
  lcd.clear();
  lcd.setCursor(0, 0);



  if (percentMoisture > 90) {
    lcd.print("Soil: ");
    lcd.print(percentMoisture);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Need Water");

    B.print("Soil: ");
    B.print(percentMoisture);
    B.print("%");
  
    B.print("Need Water");

    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  } else {
    lcd.print("Soil : ");
    lcd.print(percentMoisture);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("No Water Needed");

    B.print("Soil : ");
    B.print(percentMoisture);
    B.print("%");
   
    B.print("No Water Needed");

    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  // Print soil moisture percentage value to Serial Monitor
  Serial.print("Soil : ");
  Serial.print(percentMoisture);
  Serial.println("%");

  delay(2000); // Wait for 2 seconds before continuing

  // Read analog value from LDR pin
  int ldrValue = analogRead(LDR_PIN);

  // Convert analog reading of a photoresistor to a voltage reading and calculate Lux value
  float voltage = analogRead(A0) * (5.0 / 1023.0); // Read voltage from analog pin and convert to volts
  float lux = 500 / ((10000 * (5 - voltage)) / voltage); // Calculate the Lux value assuming a 10K Ohm resistor

  // Set cursor position on first row, first column of LCD display and print light intensity value
  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(ldrValue);

  
  B.print("Light: ");
  B.print(ldrValue);

  // Set cursor position on second row, first column of LCD display and print Lux value
  lcd.setCursor(0, 1);
  lcd.print("Lux: ");
  lcd.print(lux);
  lcd.print(" lm/m2"); // Print Lux value with units


  B.print("Lux: ");
  B.print(lux);
  B.print(" lm/m2"); 

  delay(2000); // Wait for 2 seconds before continuing
}