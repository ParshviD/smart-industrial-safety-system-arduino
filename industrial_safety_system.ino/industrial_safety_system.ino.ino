#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ================= PIN DEFINITIONS =================

// 🔥 CENTRAL FIRE ZONE
#define FLAME_PIN 9
#define CENTRAL_BUZZER 10

// 🟥 ZONE A – COMBUSTIBLE
#define MQ2_PIN A2
#define LM35_A  A0
#define ZONEA_BUZZER 11

// 🟦 ZONE B – RAW MATERIAL / FOOD
#define IR_PIN 5
#define DHT_PIN 2

// 🟨 ZONE C – WAREHOUSE
#define LM35_C A1
#define VIB_PIN 7

// ================= CONSTANTS =================
#define DHTTYPE DHT11
#define HUMIDITY_LIMIT 40
#define MQ2_THRESHOLD 715
#define TEMP_LIMIT_A  205
#define TEMP_LIMIT_C  215

// ================= OBJECTS =================
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHTTYPE);

// ================= TIMING =================
unsigned long startTime;
unsigned long vibTime = 0;   // for vibration latch

// ================= LM35 FUNCTION =================
float readLM35(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  float avg = sum / 10.0;
  float voltage = avg * (5.0 / 1023.0);  // 5V reference
  return voltage * 100.0;                // LM35: 10mV/°C
}

// ================= SETUP =================
void setup() {

  pinMode(FLAME_PIN, INPUT_PULLUP);   // flame sensor is ACTIVE-LOW
  pinMode(CENTRAL_BUZZER, OUTPUT);
  pinMode(ZONEA_BUZZER, OUTPUT);
  pinMode(VIB_PIN, INPUT);

  digitalWrite(CENTRAL_BUZZER, LOW);
  digitalWrite(ZONEA_BUZZER, LOW);

  Serial.begin(9600);
  Serial.println("Industrial Safety System Started");

  lcd.init();
  lcd.backlight();
  dht.begin();

  // ---- LCD STARTUP SEQUENCE ----
  lcd.setCursor(0, 0);
  lcd.print("ZONE B");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ZONE C");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");
  delay(2000);

  lcd.clear();

  startTime = millis();   // for flame sensor stabilization
}

// ================= LOOP =================
void loop() {

  // ---------- 🔥 CENTRAL FIRE ZONE ----------
  if (millis() - startTime > 3000) {     // allow sensor to stabilize
    if (digitalRead(FLAME_PIN) == LOW) {
      digitalWrite(CENTRAL_BUZZER, HIGH);
      Serial.println("CENTRAL FIRE ALERT");
    } else {
      digitalWrite(CENTRAL_BUZZER, LOW);
    }
  }

  // ---------- 🟥 ZONE A ----------
  int smoke = analogRead(MQ2_PIN);
  float tempA = readLM35(LM35_A);

  if (smoke > MQ2_THRESHOLD || tempA > TEMP_LIMIT_A) {
    digitalWrite(ZONEA_BUZZER, HIGH);
  } else {
    digitalWrite(ZONEA_BUZZER, LOW);
  }

  // ---------- 🟦 ZONE B ----------
  float humidity = dht.readHumidity();
  int irState = digitalRead(IR_PIN);

  // ---------- 🟨 ZONE C ----------
  float tempC = readLM35(LM35_C);

  // ----- VIBRATION LATCH LOGIC -----
  if (digitalRead(VIB_PIN) == HIGH) {
    vibTime = millis();   // remember vibration time
  }
  bool vibAlert = (millis() - vibTime < 3000); // 3-second latch

  // ---------- SERIAL MONITOR (AS BEFORE) ----------
  Serial.print("Zone A -> Smoke: ");
  Serial.print(smoke);
  Serial.print(" | TempA: ");
  Serial.print(tempA);
  Serial.println(" C");

  Serial.print("Zone B -> Humidity: ");
  Serial.print(humidity);
  Serial.print("% | IR: ");
  Serial.println(irState == LOW ? "OBJECT" : "CLEAR");

  Serial.print("Zone C -> TempC: ");
  Serial.print(tempC);
  Serial.print(" C | Vibration: ");
  Serial.println(vibAlert ? 1 : 0);

  Serial.println("--------------------------------");

  // ---------- LCD DISPLAY ----------
  lcd.setCursor(0, 0);

  // Humidity (Zone B)
  if (humidity > HUMIDITY_LIMIT) {
    lcd.print("HIGH HUM  ");
  } else {
    lcd.print("Hum:");
    lcd.print((int)humidity);
    lcd.print("%   ");
  }

  // Temperature (Zone C)
  if (tempC > TEMP_LIMIT_C) {
    lcd.print("HIGH T");
  } else {
    lcd.print("T:");
    lcd.print((int)tempC);
    lcd.print("C");
  }

  lcd.setCursor(0, 1);

  // IR (Zone B)
  if (irState == LOW) {
    lcd.print("Object    ");
  } else {
    lcd.print("Area Clr  ");
  }

  // Vibration (Zone C, latched, no colon)
  if (vibAlert) {
    lcd.print("VIB ALERT");
  } else {
    lcd.print("Vib OK");
  }

  delay(2000);
}