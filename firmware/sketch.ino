#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4       
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define PIR_PIN 15     
#define LDR_PIN 34     
#define BUZZER_PIN 27  

#define RED_PIN 12     
#define GREEN_PIN 13   
#define BLUE_PIN 14    


void setColor(int red, int green, int blue);

void setup() {
  Serial.begin(115200); 
  
  
  analogReadResolution(10); 
  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 OLED initialization failed!"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  
  dht.begin();
  
 
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
 
  setColor(0, 255, 0); 
  tone(BUZZER_PIN, 1500, 200);
  delay(200);
  setColor(0, 0, 0);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int pirState = digitalRead(PIR_PIN);
  int ldrVal = analogRead(LDR_PIN); 

  if (isnan(temp) || isnan(hum)) {
    temp = 0.0;
    hum = 0.0;
  }

  
  if (pirState == HIGH) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.print("WARNING!");
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.print("Motion Detected!");
    display.display();
    
    for (int i = 0; i < 3; i++) {
      setColor(255, 0, 0); 
      tone(BUZZER_PIN, 1000);
      delay(150);
      setColor(0, 0, 0);
      noTone(BUZZER_PIN);
      delay(150);
    }
  } 
  
  
  else if (ldrVal < 300) {
    setColor(0, 50, 150); 
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("--- NIGHT MODE ---");
    display.setCursor(0, 20);
    display.print("Temp: "); display.print(temp, 1); display.print(" C");
    display.setCursor(0, 35);
    display.print("Hum : "); display.print(hum, 1); display.print(" %");
    display.setCursor(0, 50);
    display.print("LDR Value: "); display.print(ldrVal);
    display.display();
    
    noTone(BUZZER_PIN);
    delay(1000); 
  } 
  
 
  else {
    setColor(0, 0, 0); 
    noTone(BUZZER_PIN);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("--- SMART DESK ---");
    
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(temp, 1); display.write(247); display.print("C"); 
    
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("Humidity: "); display.print(hum, 1); display.print("%");
    display.setCursor(0, 55);
    display.print("Status: Active");
    display.display();
    
    delay(1000);
  }
}


void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}