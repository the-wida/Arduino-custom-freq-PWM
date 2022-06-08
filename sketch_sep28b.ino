#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     12

#include <Arduino.h>
#include "Adafruit_SSD1306.h"
#include <Wire.h>

float  periode;
float frequency = 100;
int  percentageDuty = 50;
unsigned long periodeHigh;
unsigned long startmicros = micros();
bool dutyOn = true;
int rotaryInOld;
int rotaryIn;
bool clicked = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Click();
void Rotate();
void OledFreq();
void OledDuty();

void setup() 
{
//PWM OUT PIN
pinMode (6, OUTPUT);
Serial.begin (9600);
rotaryInOld = digitalRead (3);
display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
OledDuty();
attachInterrupt (digitalPinToInterrupt(2), Click, FALLING);
}
void loop()
{
//PERIODE SETUP
periode = ((float)1/frequency)*1000000;//microseconds
periodeHigh =  (periode * percentageDuty)/(float)100;//microseconds
//PWM HIGH
if (micros() - startmicros <=periodeHigh)
{
  digitalWrite (6, HIGH);
}
//PWM LOW
else if (micros() - startmicros <= periode)
{
  digitalWrite (6, LOW);
}
//PERIODE END
else
{
  startmicros = micros();
}
rotaryIn = digitalRead (3);
if (rotaryInOld != rotaryIn)
{
  Rotate();
  rotaryInOld = rotaryIn;
}
if (clicked == true)
{
  if (dutyOn)
  {
    OledDuty();
  }
  else
  {
    OledFreq();
  }
  clicked = false;
}
}


void Click()
{
  dutyOn = !dutyOn;
  clicked = true;
}
void Rotate()
{
        if (digitalRead(4) == rotaryIn) //right
         { 
            if (dutyOn && percentageDuty<100)
            {
              percentageDuty++;
            }
            else if (!dutyOn  && frequency < 1000)
            {
              frequency+=10;
            }    
         }
         else //left
         { 
           if (dutyOn && percentageDuty>0)
            {
              percentageDuty--;
            }
            else if (!dutyOn  && frequency > 100)
            {
              frequency -=10;
            }    
         }
         display.clearDisplay();
         display.display(); 
         display.setCursor(0,10);
         display.print("Frequency = ");
         display.print(frequency);
         display.println(" Hz");
         display.print("Duty = ");
         display.print(percentageDuty);
         display.print(" %");
         display.display(); 
}


void OledFreq()
{
  display.clearDisplay();
  display.display(); 
  display.setCursor(0,10);
  display.println("Change Frequency...");
  display.display(); 
}

void OledDuty()
{
  display.clearDisplay();
  display.display(); 
  display.setCursor(0,10);
  display.println("Change Duty...");
  display.display(); 
}
