#include <DHT.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>
#define DHTTYPE DHT11

const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int rainSensorPin = 12;
const int DHTPIN = 11;
const int waterSensorPin = A3;
const int soilMoisturePin = A2;
const int pumpPin = 13; 
const int TankPump = 4;

int waterlevel = 0;
int wet = 80;
int dry = 50;

DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;

void setup() {
  pinMode(rainSensorPin, INPUT);
  pinMode(pumpPin, OUTPUT); // Set the pump control pin as output
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Irrigation system!!!");
  delay(500);

  dht.begin();
  
  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC lost power, ");
    lcd.setCursor(1, 0);
    lcd.print("setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.begin(9600);
}

void loop() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Taking readings....");
  lcd.setCursor(0, 1);
  lcd.print("                   ");
  lcd.setCursor(0, 0);
  lcd.print("crop_planted  ");
  lcd.print(crop_planted);
  lcd.print(",");
  lcd.print(21);
  lcd.print(",");
  lcd.print("Soil Moisture  ");
  lcd.print(soilMoisture);
  lcd.print(",");
  lcd.print("temparature  ");
  lcd.print(temperature);
  lcd.print(",");
  lcd.print("humidity  ");
  lcd.print(humidity);
  
  float temperature = readTemperature();
  float humidity = readHumidity();
  int soilMoisture = readSoilMoisture(soilMoisturePin, dry, wet);
  int rainReadings = digitalRead(rainSensorPin); // Take rain readings
  waterlevel = analogRead(waterSensorPin); // Read water level

  int crop_planted = 8; // wheat

  // Send sensor data to the external ML model
  Serial.print(crop_planted);
  Serial.print(",");
  Serial.print(21);
  Serial.print(",");
  Serial.print(soilMoisture);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.println();
  
  // Wait for prediction from the ML model
  if (Serial.available() > 0) {
    int prediction = Serial.parseInt();
    Serial.print("Prediction received: ");
    Serial.println(prediction); // Debug print

    if (prediction == 1) {
      if (rainReadings == 0) {
        // If no rain is detected
        lcd.setCursor(0, 0);
        lcd.print("No rain detected");
        delay(500);
        lcd.setCursor(0, 0);
        lcd.print("Please wait.....");
        delay(100);
        if ((now.hour() >= 6 && now.hour() <= 9) || (now.hour() >= 16 && now.hour() <= 21)) {
        
          // Irrigation is needed, turn on pump
          if (waterlevel < 20) {
            // If water level is below 20, fill the tank
            lcd.setCursor(0, 0);
            lcd.print("Filling tank...");
            digitalWrite(TankPump, HIGH);
            while (waterlevel < 70) {
              delay(1000);
              waterlevel = analogRead(waterSensorPin);
              lcd.setCursor(0, 1);
              lcd.print("Level: ");
              lcd.print(waterlevel);
            }
            // Turn off pump when the water level has reached 70
            digitalWrite(TankPump, LOW);
          }
          lcd.setCursor(0, 0);
          lcd.print("Irrigating...");
          digitalWrite(pumpPin, HIGH);
          delay(1000);
          digitalWrite(pumpPin, LOW);
        } else {

          // Irrigation is not needed, turn off pump
          digitalWrite(pumpPin, LOW);
          digitalWrite(TankPump, LOW);
          lcd.setCursor(0, 0);
          lcd.print("No need to irrigate");
          delay(100);
        }
      }
      else{
          // It is raining and there is no need to irrigate
          digitalWrite(pumpPin, LOW);
          digitalWrite(TankPump, LOW);
          lcd.setCursor(0, 0);
          lcd.print("Rain detected");
          lcd.setCursor(1, 0);
          lcd.print("No need to irrigate");
          delay(100);
      }
    } 
    else {
        // The model predicted that the system should not irrigate
        lcd.setCursor(0, 0);
        lcd.print("No need to irrigate");
        delay(100);      
    }
  }

  //delay(100);
}

float readTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed to read");
    lcd.setCursor(1, 0);
    lcd.print("from DHT sensor!");
    return -1; // Return a default value
  }
  return t;
}

float readHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed to read");
    lcd.setCursor(1, 0);
    lcd.print("from DHT sensor!");
    return -1; // Return a default value
  }
  return h;
}

int readSoilMoisture(int soilMoisturePin, int dry, int wet) {
  int soilMoisture = analogRead(soilMoisturePin);
  int soilMoisturePercent = map(soilMoisture, dry, wet, 0, 100);
  return soilMoisturePercent;
}
