/*************************************************************
TeaTime v1.0

 *************************************************************/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "keys.h"

int servo_pin = 2;
int led_pin = 4;
int led_pixels = 1;
int timer_sec = 0;
int guage_val = 0;
bool brew_state = false;
int brew_time = 0;
int brew_start = 0;
int brew_temp = 0;
int servo_up = 105;
int servo_down = 45;
int servo_angle = servo_up;
int servo_current = 0;

String brew_tea = "Tea";

Servo servo;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(led_pixels, led_pin, NEO_GRB + NEO_KHZ800);
WidgetLCD lcd(V8);


// V0 = Servo Angle Input
// V1 = timer
// V2 = tea state
// V3 = Black Tea (100C, 3 mins)
// V4 = Oolong Tea (95C, 3 mins)
// V5 = Green and White Tea (80C, 3 mins)
// V6 = Roibos Tea (100C 5-7mins)
// V7 = Guage (0-100%)
// V8 = LCD Screen

BLYNK_WRITE(V0)
{
  servo_angle = param.asInt();
}

BLYNK_WRITE(V2)
{
  if(param.asInt()){
    brew_state = false;
  }
  else{
    brew_state = true;
  }
 
}

BLYNK_WRITE(V3) //Black Tea
{
  if(param.asInt()){
    brew_tea = "Black Tea";
    brew_temp = 100;
    int brew_mins = 3;
    brew_time = brew_mins*60*1000;

    brew_state = true;
    brew_start = millis();

    servo_angle = servo_down;
    Serial.println("Start Brewing " + brew_tea);
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(1, 0, brew_tea); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(1, 1, String(brew_mins) + "mins at " + String(brew_temp) + "C");


  }
}

BLYNK_WRITE(V4) //Oolong Tea
{
  if(param.asInt()){
    brew_tea = "Oolong Tea";
    brew_temp = 95;
    int brew_mins = 3;
    brew_time = brew_mins*60*1000;

    brew_state = true;
    brew_start = millis();

    servo_angle = servo_down;
    Serial.println("Start Brewing " + brew_tea);
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(1, 0, brew_tea); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(1, 1, String(brew_mins) + "mins at " + String(brew_temp) + "C");


  }
}

BLYNK_WRITE(V5) //Green and White Tea
{
  if(param.asInt()){
    brew_tea = "Green Tea";
    brew_temp = 80;
    int brew_mins = 3;
    brew_time = brew_mins*60*1000;

    brew_state = true;
    brew_start = millis();

    servo_angle = servo_down;
    Serial.println("Start Brewing " + brew_tea);
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(1, 0, brew_tea); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(1, 1, String(brew_mins) + "mins at " + String(brew_temp) + "C");


  }
}

BLYNK_WRITE(V6) //Roibos Tea
{
  if(param.asInt()){
    brew_tea = "Rooibos Tea";
    brew_temp = 100;
    int brew_mins = 6;
    brew_time = brew_mins*60*1000;

    brew_state = true;
    brew_start = millis();

    servo_angle = servo_down;
    Serial.println("Start Brewing " + brew_tea);
    lcd.clear(); //Use it to clear the LCD Widget
    lcd.print(1, 0, brew_tea); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
    lcd.print(1, 1, String(brew_mins) + "mins at " + String(brew_temp) + "C");


  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pixels.begin();
  servo.attach(servo_pin);
  pixels.setPixelColor(1, pixels.Color(0,0,0));
  pixels.show();
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(1, 0, "Select a Tea"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(1, 1, "");

}

void loop()
{
  Blynk.run();
  if(brew_state){
    servo_angle = servo_down;
    pixels.setPixelColor(1, pixels.Color(255,0,0));
    timer_sec = (brew_start + brew_time-millis())/1000;
    if(brew_start + brew_time < millis()){
      brew_state = false;
      lcd.clear(); //Use it to clear the LCD Widget
      lcd.print(1, 0, "Select a Tea"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
      lcd.print(1, 1, "");
    }
  }
  else{
    servo_angle = servo_up;
    timer_sec = 0;
    brew_temp = 0;
    pixels.setPixelColor(1, pixels.Color(0,255,0));
  }

  
  servo.write(servo_angle);
  Serial.println("Servo angle: " + String(servo_angle));
  pixels.show(); 


  //Write to displays
  if (timer_sec>1000){
    timer_sec = 0;
  }
  Blynk.virtualWrite(V1, timer_sec);
  // Blynk.virtualWrite(V2, brew_state);
  Blynk.virtualWrite(V7, brew_temp);

  delay(100);
  Serial.println("brew time:" + String(brew_time));
  Serial.println("timer sec:" + String(timer_sec));

}
