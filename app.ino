#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define capacitiveSoil A0
#define in1 3
#define in2 5
#define ENA 6
#define switchKanan 7
#define switchKiri 8
#define buzzer 9

int period = 4000;
unsigned long time_now = 0;

int kadarAir;
int persen;

bool gerakKiri;
bool udah_bunyi = false;
bool stop = false;

void setup()
{
    Serial.begin(9600);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(ENA, OUTPUT);

    pinMode(switchKanan, INPUT_PULLUP);
    pinMode(switchKiri, INPUT_PULLUP);

    pinMode(buzzer, OUTPUT);

    Wire.begin();
    Wire.beginTransmission(0x27);
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    // Serial.print("Kanan ");
    // Serial.print("\t");
    // Serial.print("Kiri ");
}

void loop()
{
    getData();
    analogWrite(ENA, 255);
    home();
    if (switchKanan == LOW && switchKiri == HIGH)
    {
        gerakKiri = true;
    }
    else if (switchKanan == HIGH && switchKiri == LOW)
    {
        gerakKiri = false;
    }

    if (gerakKiri == true)
    {
        if (stop == false)
        {
            motorKiri();
            delay(200);
        }
        stop = true;
        if (stop == true)
        {
           motorStop();
        }
        if (millis() > time_now + period)
        {
            time_now = millis();
            stop = false;
        }
    }
    else if (gerakKiri == false)
    {
        if (stop == false)
        {
            motorKanan();
            delay(200);
        }
        stop = true;
        if (stop == true)
        {
           motorStop()
        }
        if (millis() > time_now + period)
        {
            time_now = millis();
            stop = false;
        }
    }
    bacaSensor();
}

void getData()
{
    kadarAir = analogRead(capacitiveSoil);
    persen = -0.04 * kadarAir + 52;
    Serial.print(persen);
    Serial.println("%");
    Serial.print("\t\t");
    Serial.println(kadarAir);
    if (millis() > time_now + period)
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
}

void bacaSensor()
{
    if (persen <= 25 && udah_bunyi == false)
    {
        bunyi();
        udah_bunyi = false;
    }
    else if (persen > 25)
    {
        udah_bunyi = false;
    }
}

void motorKanan()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
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
}

void bunyi()
{
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
}

void home()
{
    if (digitalRead(switchKiri) == HIGH && digitalRead(switchKanan) == HIGH)
    {
        motorKanan();
    }
}
