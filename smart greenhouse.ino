// ==========================================
//         BLYNK VE WIFI AYARLARI
// ==========================================
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// ==========================================
//          PIN VE DONANIM AYARLARI
// ==========================================
#define DHTPIN 32     
#define LED_PIN 5      
#define FAN_PIN 16    
#define LDR_PIN 34    
#define DHTTYPE DHT11 

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// ==========================================
//               NESNELER VE DEĞİŞKENLER
// ==========================================
LiquidCrystal_I2C lcd(0x27, 20, 4); 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer; 

int hedefSicaklik = 26; 
int hedefIsik = 30;     

// ==========================================
//           BLYNK VERİ OKUMA (INPUT)
// ==========================================
BLYNK_WRITE(V3) {
  hedefSicaklik = param.asInt();
  Serial.print("Yeni Hedef Sicaklik: "); 
  Serial.println(hedefSicaklik);
}

BLYNK_WRITE(V4) {
  hedefIsik = param.asInt();
  Serial.print("Yeni Hedef Isik: "); 
  Serial.println(hedefIsik);
}

// ==========================================
//            ANA SENSÖR DÖNGÜSÜ
// ==========================================
void sensorDongusu() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int ldrRaw = analogRead(LDR_PIN);
  int isikYuzde = map(ldrRaw, 0, 4095, 0, 100); 

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Sensor Hatasi!");
    lcd.setCursor(0,0); lcd.print("SENSOR HATASI!");
    return;
  }

  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, isikYuzde);
  }

  if (isikYuzde < hedefIsik) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  if (t > hedefSicaklik) {
    digitalWrite(FAN_PIN, HIGH);
    if (Blynk.connected()) Blynk.virtualWrite(V5, 1);
  } else {
    digitalWrite(FAN_PIN, LOW);
    if (Blynk.connected()) Blynk.virtualWrite(V5, 0);
  }

  lcd.setCursor(0, 0); 
  lcd.print("S:"); lcd.print(t, 1); lcd.print("C "); 
  lcd.setCursor(10, 0); 
  lcd.print("Hdf:"); lcd.print(hedefSicaklik); lcd.print("C  "); 

  lcd.setCursor(0, 1);
  lcd.print("I:%"); lcd.print(isikYuzde); lcd.print("  "); 
  lcd.setCursor(10, 1); 
  lcd.print("Hdf:%"); lcd.print(hedefIsik); lcd.print("  "); 
  
  lcd.setCursor(0, 2);
  lcd.print("Fan: ");
  if(digitalRead(FAN_PIN)) lcd.print("ACIK  "); 
  else lcd.print("KAPALI"); 
  
  lcd.setCursor(0, 3);
  if (Blynk.connected()) {
    lcd.print("Durum: ONLINE       ");
  } else {
    lcd.print("Durum: BAGLANIYOR...");
  }
}

// ==========================================
//                 KURULUM (SETUP)
// ==========================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  dht.begin();
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0); lcd.print("Sistem Baslatiliyor");
  lcd.setCursor(0,1); lcd.print("IoT Akilli Sera"); // İsim yerine proje ismini yazdık
  
  Blynk.config(BLYNK_AUTH_TOKEN);
  WiFi.begin(ssid, pass); 
  
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 3000) {
    delay(500);
  }
  
  if(WiFi.status() == WL_CONNECTED){
     Blynk.connect();
  }
  
  lcd.clear();
  timer.setInterval(1000L, sensorDongusu);
}

// ==========================================
//                 ANA DÖNGÜ (LOOP)
// ==========================================
void loop() {
  if (Blynk.connected()) {
    Blynk.run();
  } else {
    static unsigned long sonDeneme = 0;
    if (millis() - sonDeneme > 15000) {
      sonDeneme = millis();
      if(WiFi.status() != WL_CONNECTED){
         WiFi.reconnect();
      } else {
         Blynk.connect();
      }
    }
  }
  timer.run();
}