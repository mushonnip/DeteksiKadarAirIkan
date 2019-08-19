#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define capacitiveSoil A0
#define in1 3
#define in2 5
#define ENA 6
#define switchKanan 7
#define switchKiri 8
#define buzzer 9
#define relay 10

int stepKiri = 0;
int stepKanan = 0;

int period = 60000;
int satu = 1000;
unsigned long time_now = 0;
unsigned long time_now2 = 0;
unsigned long time_now3 = 0;

int kadarAir;
int persen;

bool udah_bunyi = false;
bool stop = false;
bool autoMode = true;

bool gerakKanan = false;
bool gerakKiri = false;

void setup()
{
    Serial.begin(9600);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(relay, OUTPUT);

    pinMode(switchKanan, INPUT_PULLUP);
    pinMode(switchKiri, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);

    lcd.begin();
    lcd.backlight();
    // Serial.print("Kanan ");
    // Serial.print("\t");
    // Serial.print("Kiri ");
}

void loop()
{
    kadarAir = analogRead(capacitiveSoil);
    persen = -0.04 * kadarAir + 52;
    Serial.print(persen);
    Serial.println("%");
    Serial.print("\t\t");
    Serial.println(kadarAir);
    if (millis() > time_now + satu)
    {
        time_now = millis();
        lcd.setCursor(0, 0);
        lcd.print(persen);
        lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.print(kadarAir);
    }

    // Serial.print(digitalRead(switchKanan));
    // Serial.print("\t");
    // Serial.println(digitalRead(switchKiri));

    if (persen <= 25 && udah_bunyi == false)
    {
        bunyi();
        udah_bunyi = false;
        lcd.setCursor(5, 1);
        lcd.print("Kering");
    }
    else if (persen > 25)
    {
        udah_bunyi = false;
        lcd.setCursor(5, 1);
        lcd.print("Basah");
    }

    if (stepKiri <= 98 && digitalRead(switchKanan) == HIGH)
    {

        lcd.setCursor(5, 0);
        lcd.print("<-");
        motorKiri();
        delay(200);
        motorStop();
        digitalWrite(relay, LOW);
        if (millis() > time_now2 + period)
        {
            time_now2 = millis();
            digitalWrite(relay, HIGH);
            stepKiri += 1;
        }
        lcd.setCursor(9, 0);
        lcd.print(stepKiri);
        
    }

    if (stepKiri > 98 && stepKanan <= 98 && digitalRead(switchKanan) == HIGH)
    {
        lcd.setCursor(5, 0);
        lcd.print("->");
        motorKanan();
        delay(200);
        motorStop();
        digitalWrite(relay, LOW);
        if (millis() > time_now3 + period)
        {
            time_now3 = millis();
            digitalWrite(relay, HIGH);
            stepKanan += 1;
        }
        lcd.setCursor(9, 0);
        lcd.print(stepKanan);
    }

    if (digitalRead(switchKanan) == LOW)
    {
        digitalWrite(relay, HIGH);
        motorKanan();
        lcd.setCursor(5, 0);
        lcd.print("->");
    }

}

void motorKanan()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(ENA, 255);
}

void motorStop()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
}

void motorKiri()
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(ENA, 255);
}

void bunyi()
{
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
}