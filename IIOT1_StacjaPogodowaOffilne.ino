#include "M5Core2.h"
#include "M5_ENV.h"
SHT3X sht30;
QMP6988 qmp6988;
RTC_TimeTypeDef RTCTime;
RTC_DateTypeDef RTCDate;
int8_t menu_stan=1; // Display page
int8_t pomiary_stan=0;
char disp_refresh=1; // Display refresh
int drawScreen = 1;
long timer1,timer2 = 0;
int timerLimit1 = 5000,timerLimit2 = 25000;

float getPressure();
float getTemperature();
float getHumidity();
void readSystemDate();
void readSystemTime();
void setDate();
int getYear();
int getMonth();
int getDay();
int getHours();
int getMinutes();
void screenNav();
void screen1();
void screen2();
void screen3();
void screen4();
void screen5();
void screen6();
void screen7();
void screen8();
void countTimers();
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Wire init, adding the I2C bus.
  qmp6988.init();
  M5.begin();
  M5.Lcd.setTextSize(5);
  setDate();
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if(disp_refresh){ M5.Lcd.fillScreen(BLACK); disp_refresh = 0; }

  switch(menu_stan)
  {
  case 1:
  screen1();
  break;
  case 2:
  screen2();
  break;
  case 3:
  screen3();
  break;
  case 4:
  screen4();
  break;
  case 5:
  screen5();
  break;  
  case 6:
  screen6();
  break;  
  case 7:
  screen7();
  break;  
  case 8:
  screen8();
  break;    
  }
  


}

float getTemperature()
{
  if(sht30.get()==0){return sht30.cTemp; }else{return 0;}
}
float getPressure()
{
  return qmp6988.calcPressure()/100;
}
float getHumidity()
{
  if(sht30.get()==0){return sht30.humidity; }else{return 0;}
}
void readSystemDate() { M5.Rtc.GetDate(&RTCDate); }
void readSystemTime() { M5.Rtc.GetTime(&RTCTime); }
void setDate() {
  RTCTime.Hours = 10; //Set the time.  设置时间
  RTCTime.Minutes = 49;
  RTCTime.Seconds = 0;
  M5.Rtc.SetTime(&RTCTime); //and writes the set time to the real time clock.  并将设置的时间写入实时时钟

  RTCDate.Year = 2023;  //Set the date.  设置日期
  RTCDate.Month = 9;
  RTCDate.Date = 15;
  M5.Rtc.SetDate(&RTCDate);}
int getYear() {readSystemDate();return RTCDate.Year;}
int getMonth() {readSystemDate();return RTCDate.Month;}
int getDay() {readSystemDate();return RTCDate.Date;}
int getHours() {readSystemTime();return RTCTime.Hours;}
int getMinutes() {readSystemTime();return RTCTime.Minutes;}
void screenNav()
{

  
  
  //if (M5.BtnA.wasPressed()) {M5.Lcd.fillScreen(BLACK); wyswietl=1;menu_stan=0;}
  //else if (M5.BtnB.wasPressed()) {M5.Lcd.fillScreen(BLACK); wyswietl=1;menu_stan=1;}
  //else if (M5.BtnC.wasPressed()) {M5.Lcd.fillScreen(BLACK); wyswietl=1;menu_stan=2;}
}
void countTimers() {timer1 = millis(); timer2 = millis();}

void screen1()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("%d.%d.%d", getDay(), getMonth(),getYear());
    M5.Lcd.setCursor(0,70);
    M5.Lcd.printf("%d:%d", getHours(), getMinutes());
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 2; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen2()
{
  if (drawScreen) {
    drawScreen--;
    float temp = getTemperature();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Temp.");
    M5.Lcd.setCursor(0,70);
    M5.Lcd.printf("%.1f C", temp);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 3; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen3()
{
  if (drawScreen) {
    drawScreen--;
    float pres = getPressure();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Pressure");
    M5.Lcd.setCursor(0,70);
    M5.Lcd.printf("%.0f hPa", pres);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 4; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen4()
{
  if (drawScreen) {
    drawScreen--;
    float hum = getHumidity();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Humidity");
    M5.Lcd.setCursor(0,70);
    M5.Lcd.printf("%.0f %%", hum);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 1; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen5()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Return");
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 8; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasPressed()) {menu_stan = 1; drawScreen=1; timer1 = millis(); timer2=0;}
    else if (M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen6()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Stats");
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 5; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 7; drawScreen=1; timer2 = millis();}
}
void screen7()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Options");
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 8; drawScreen=1; timer2 = millis();}
}
void screen8()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.printf("Forecast");
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 7; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 5; drawScreen=1; timer2 = millis();}
}
