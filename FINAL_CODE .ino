#define BLYNK_TEMPLATE_ID "TMPL4AI8GZwqI"
#define BLYNK_TEMPLATE_NAME "TEMP AND HUM"
#define BLYNK_AUTH_TOKEN "P6a5OovxY4b4a4_Pn_PG4ivQZMFvD3rY"
#define BLYNK_PRINT Serial
#define DHTPIN 2 //Connect Out pin 2 in esp32
#define DHTTYPE DHT22
#define   LAMP  27
#define   COOL_FAN  16
#define   AIR_FAN     17
#define   ROTATE 14
#define   LAMP_ON       digitalWrite(LAMP,LOW);
#define   LAMP_OFF      digitalWrite(LAMP,HIGH);
#define   COOLER_ON     digitalWrite(COOL_FAN,LOW);
#define   COOLER_OFF    digitalWrite(COOL_FAN,HIGH);
#define   AIR_FAN_ON    digitalWrite(AIR_FAN,LOW);
#define   AIR_FAN_OFF   digitalWrite(AIR_FAN,HIGH);
#define   ROTATE_ON     digitalWrite(ROTATE,LOW);
#define   ROTATE_OFF    digitalWrite(ROTATE,HIGH);
#define   ROTATION_STEP    2

// Include the libraries:
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp32.h>

long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second

unsigned long previous_turn = 0;
const unsigned long interval_turn = ROTATION_STEP * 60 * 1000; // 30 minutes in milliseconds
const unsigned long Tzone1 = 8 * 24 * 60 * 60 * 1000; // TIME ZONE_1 in milliseconds
const unsigned long Tzone2 = 15 * 24 * 60 * 60 * 1000; // TIME ZONE_2 in milliseconds
const unsigned long Tzone3 = 18 * 24 * 60 * 60 * 1000; // TIME ZONE_3 in milliseconds
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "COSMOTALK";  // type your wifi name
char pass[] = "ece01671";  // type your wifi password
//char ssid[] = "      ";  // type your wifi name
//char pass[] = "";  // type your wifi password
float temp_h, temp_t,t,h;
int wait = 5000;

BlynkTimer timer;

// Initialize DHT sensor
DHT dht = DHT(DHTPIN, DHTTYPE);


// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;
// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
byte chicken[] = { B01110, B10110, B01100, B11001, B11111, B11111, B01010, B10100 };
byte egg[] = { B00000, B01110, B11111, B11111, B11111, B11111, B01110, B00000 };

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //
  delay(wait);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
}

void LcdValue(float h, float t,float temp_h, float temp_t) {
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);
  // print message
  lcd.write(0);
  lcd.setCursor(1, 0);
  lcd.write(0);
  lcd.setCursor(2, 0);
  lcd.print(" EGG  INCUBATOR");
  lcd.setCursor(18, 0);
  lcd.write(1);
  lcd.write(1);
  // set cursor to first column, second row
  lcd.setCursor(0, 1);
  lcd.print("BY ECE1671-1695-1753");
  lcd.setCursor(0, 2);
  lcd.print("TEMP:");
  lcd.setCursor(11, 2);
  lcd.print("HUM:");
  lcd.setCursor(5, 2);
  if (isnan(t)) {
    lcd.print(temp_t); 
  }
  else{
    lcd.print(t);
  }
  lcd.setCursor(15, 2);
  if (isnan(h)) {
    lcd.print(temp_h); 
  }
  else{
  lcd.print(h);
  }
}

void RotateEgg() {
  ROTATE_ON;
  delay(9000);
  ROTATE_OFF;
}


void DayTime() {
  long timeNow = millis();
  int days = timeNow / day ;                                //number of days
  int hours = (timeNow % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((timeNow % day) % hour) / minute ;         //and so on...
  int seconds = (((timeNow % day) % hour) % minute) / second;

  // digital clock display of current time
  lcd.setCursor(0, 3);
  lcd.print(days);
  lcd.print("days ");
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds);
}

void setup()
{
  Serial.begin(115200);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.createChar(0, chicken);
  lcd.createChar(1, egg);

  // Setup sensor:
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);

  //pin_setup
  pinMode(LAMP, OUTPUT);
  pinMode(COOL_FAN, OUTPUT);
  pinMode(AIR_FAN, OUTPUT);
  pinMode(ROTATE, OUTPUT);

  digitalWrite(ROTATE, HIGH); // Turn the relay OFF
  digitalWrite(COOL_FAN, HIGH); // Turn the relay OFF
  digitalWrite(AIR_FAN, HIGH); // Turn the relay OFF
  digitalWrite(LAMP, HIGH); // Turn the relay OFF
}

void loop()
{
  // get the current time in milliseconds
  unsigned long currentMillis = millis();
  h = dht.readHumidity();
  t = dht.readTemperature(); //
  if (!isnan(h)) {
   temp_h=h;
  }
    if (!isnan(t)) {
   temp_t=t;
  }
//  while (isnan(h) || isnan(t) || h==0 || t==0) {
//    delay(wait);
//     h = dht.readHumidity();
//     t = dht.readTemperature(); //
//  }
  Blynk.run();
  timer.run();
  LcdValue(h, t,temp_h,temp_t);
  DayTime();



  if (currentMillis  < Tzone1) {
    if (t < 37.5) {
      LAMP_ON
    }
    if (t >= 38) {
      LAMP_OFF
    }
    if (t > 38.5) {
      COOLER_ON
    }
    if (t < 38.4) {
      COOLER_OFF
    }
    if (h > 55) {
      AIR_FAN_ON
    }
    if (t < 51) {
      AIR_FAN_OFF
    }


    // check if the interval has passed
    if (currentMillis - previous_turn >= interval_turn) {
      // save the current time for the next iteration
      previous_turn = currentMillis;
      // call your function here
      RotateEgg();
    }
  }

  if (currentMillis  < Tzone2 && currentMillis  >= Tzone1) {
    if (t < 37.5) {
      LAMP_ON
    }
    if (t >= 38) {
      LAMP_OFF
    }
    if (t > 38.5) {
      COOLER_ON
    }
    if (t < 38.4) {
      COOLER_OFF
    }

    if (h > 60) {
      AIR_FAN_ON
    }
    if (t < 56) {
      AIR_FAN_OFF
    }
    // check if the interval has passed
    if (currentMillis - previous_turn >= interval_turn) {
      // save the current time for the next iteration
      previous_turn = currentMillis;
      // call your function here
      RotateEgg();
    }
  }

  if (currentMillis  < Tzone3 && currentMillis  >= Tzone2) {
    if (t < 37) {
      LAMP_ON
    }
    if (t >= 37.5) {
      LAMP_OFF
    }
    if (t > 38) {
      COOLER_ON
    }
    if (t < 37.9) {
      COOLER_OFF
    }

    if (h > 70) {
      AIR_FAN_ON
    }
    if (t < 66) {
      AIR_FAN_OFF
    }
    // check if the interval has passed
    if (currentMillis - previous_turn >= interval_turn) {
      // save the current time for the next iteration
      previous_turn = currentMillis;
      // call your function here
      RotateEgg();
    }
  }

  if (currentMillis  >= Tzone3) {
    if (t < 36.6) {
      LAMP_ON
    }
    if (t >= 36.9) {
      LAMP_OFF
    }
    if (t > 37.2) {
      COOLER_ON
    }
    if (t < 37.1) {
      COOLER_OFF
    }
    if (h > 75) {
      AIR_FAN_ON
    }
    if (t < 71) {
      AIR_FAN_OFF
    }

  }

}
