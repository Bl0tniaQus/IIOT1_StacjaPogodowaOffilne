#include "M5Core2.h"
#include "M5_ENV.h"
SHT3X sht30;
QMP6988 qmp6988;
RTC_TimeTypeDef RTCTime;
RTC_DateTypeDef RTCDate;
int menu_stan=1; // Display page
int pomiary_stan=0;
char disp_refresh=1; // Display refresh
int drawScreen = 1;
long timer1,timer2 = 0;
int timerLimit1 = 5000,timerLimit2 = 25000;
float temp_UB,temp_LB,hum_UB,hum_LB,pres_UB,pres_LB;

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
//ekrany główne
void screen1(); //data i czas
void screen2(); //temperatura
void screen3(); //ciśnienie
void screen4(); //wilgotność
//ekrany menu głównego
void screen5(); //menu główne powrót
void screen6(); //menu główne statystyki
void screen7(); //menu główne ustawienia
void screen8(); //menu główne prognoza
//ekrany statystyk
//ekrany menu ustawień
void screen9();
//ekrany prognozy


void countTimers();
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Wire init, adding the I2C bus.
  M5.begin();
  setDate();
  qmp6988.init();
  temp_LB = 20; temp_UB = 24;
  hum_LB = 30; hum_UB = 50;
  pres_LB = 950; pres_UB = 1050;
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
  RTCDate.Month = 10;
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
    char czas_buf[10];
    char data_buf[15];
    M5.Lcd.setTextColor(WHITE);
    sprintf(czas_buf,"%d:%d",getHours(),getMinutes());
    sprintf(data_buf,"%d.%d.%d", getDay(), getMonth(),getYear());
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(czas_buf,160,95,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawString(data_buf,160,175,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString(".",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString(".",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(".",275,240,4);
    
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 2; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen2()
{
  if (drawScreen) {
    drawScreen--;
    float temp = getTemperature();
    char temp_buf[7];
    sprintf(temp_buf,"%.1f", temp);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    if (temp > temp_UB || temp < temp_LB) {M5.Lcd.fillRect(0, 70, 320, 110, MAROON);}
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("T [C]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(temp_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString(".",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString(".",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(".",275,240,4);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 3; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen3()
{
  if (drawScreen) {
    drawScreen--;
    float pres = getPressure();
    char pres_buf[4];
    sprintf(pres_buf,"%.0f", pres);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN);
    if (pres > pres_UB || pres < pres_LB) {M5.Lcd.fillRect(0, 70, 320, 110, MAROON);}
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("p [hPa]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(pres_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString(".",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString(".",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(".",275,240,4);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 4; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen4()
{
  if (drawScreen) {
    drawScreen--;
    float hum = getHumidity();
    char hum_buf[4];
    sprintf(hum_buf,"%.0f", hum);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(CYAN);
    if (hum > hum_UB || hum < hum_LB) {M5.Lcd.fillRect(0, 70, 320, 110, MAROON);}
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Hum. [%]",160,40,4);
    M5.Lcd.setTextSize(5);
    M5.Lcd.drawString(hum_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString(".",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString(".",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(".",275,240,4);
    }
  if ((millis()-timer1>=timerLimit1)||(millis()-timer1<0)) {menu_stan = 1; drawScreen=1; timer1=millis();}
  else if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {menu_stan = 6; drawScreen=1; timer2 = millis();}
}
void screen5()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Return",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
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
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Stats",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    
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
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Options",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
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
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Forecast",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 7; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 5; drawScreen=1; timer2 = millis();}
}
