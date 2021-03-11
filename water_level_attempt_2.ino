#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
const int trigPin = 13;
const int echoPin = 12;
const int Lval = 100;
const int Hval = 20;
const int buzpin = 7;

void setup()
{
  lcd.init();                      // initialize the lcd
  Serial.begin(9600);
  pinMode(buzpin, OUTPUT);
  digitalWrite(buzpin, HIGH);
}

void loop()
{
  long duration, cm;
  float level;

  //    lcd.setCursor(0, 0);
  lcd.backlight();
  pinMode(trigPin, OUTPUT);

  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  level = microsecondsToCentimeters(duration);
  Serial.print("Water Level: ");
  Serial.print(level);
  Serial.println("%");
  level = 100 - level;

  if (level >= 80) {
    level = 100;

  }
  if (level <= 0) {
    level = 0;
    lcd.setCursor(0, 0);
    lcd.print("Reserved Started");
  }

  if (level == 100)
  {
    int mint, sec;
    for (int j = 180; j >= 0; j--)
    {
      sec = j;
      lcd.setCursor(0, 0);
      lcd.print("Please Wait!");
      lcd.setCursor(0, 1);
      mint = j / 60;
      sec = sec - (mint * 60);
      lcd.print(mint);
      lcd.print(" : ");
      lcd.println(sec);
      delay(1000);
    }
    for (int i = 0; i < 10000; i++) {
      digitalWrite(buzpin, LOW);
      lcd.print("Tank Full!");
      lcd.backlight();
      delay(100);
      digitalWrite(buzpin, HIGH);
      lcd.clear();
      lcd.noBacklight();
      delay(100);
    }
  }
  if (level > 0) {
    lcd.setCursor(0, 0);
    lcd.print("LEVEL ");
    lcd.print(abs(level));
    lcd.print(" %       ");
  }
  lcd.setCursor(0, 1);
  lcd.print("cm: ");
  lcd.print(microsecondsToCentimeters(duration));
  lcd.print(" cm");

  delay(100);

}


long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
