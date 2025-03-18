#define BLYNK_AUTH_TOKEN "Your BLYNK_AUTH_TOKEN "
#define BLYNK_TEMPLATE_ID "Your BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Plant Watering System"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Blynk.h>
#include <WidgetRTC.h>

#define waterPump 2

LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;
bool Relay;

char auth[] = "your auth token";
const char* ssid     = "Your SSID";
const char* password = "Your Paswword";

const int soilMoisturePin = A0;
const int nemThreshold = 50; 
int nemOrani;

void sendSensorData();

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password,"blynk.cloud", 80);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  timer.setInterval(1000L, sendSensorData);
}

void soilMoisture() {
  int soilMoisture = analogRead(soilMoisturePin);
  int nemOrani = map(soilMoisture, 0, 1023, 100, 0);
  if (nemOrani < nemThreshold) {
    lcd.setCursor(0,0);
    lcd.print("TOPRAK KURU");
  } else {
    lcd.setCursor(0,0);
    lcd.print("TOPRAK NEMLI");
  }
  lcd.setCursor(0,1);
  lcd.print("Nem Orani:");
  lcd.setCursor(10,1);
  lcd.print(nemOrani);
  lcd.setCursor(13,1);
  lcd.print("%");
}

BLYNK_WRITE(V1) {
   Relay = param.asInt();
    if (Relay == 1) {
      digitalWrite(waterPump, LOW); 
    } else if(Relay == 0){
      digitalWrite(waterPump, HIGH); 
    }
    else{
      if (nemOrani < 5)
        {digitalWrite(waterPump, LOW);}
      else if (nemOrani > 70)
        {digitalWrite(waterPump, HIGH);}
    }
}

void sendSensorData() {
  int soilMoisture = analogRead(soilMoisturePin);
  int nemOrani = map(soilMoisture, 0, 1023, 100, 0);
  Blynk.virtualWrite(V0, nemOrani);
}

void loop() {
  soilMoisture();
  Blynk.run();
  timer.run();
  delay(1000);
}
