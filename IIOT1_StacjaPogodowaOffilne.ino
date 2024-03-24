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
int timerLimit1 = 5000,timerLimit2 = 10000;
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
void screen9(); //menu ustawien powrót
void screen10(); //menu ustawien data
void screen11(); //menu ustawien czas
void screen12(); //menu ustawien progi
//ekrany ustawień progów
void screen13(); //menu ustawien progów powrót
void screen14(); //temp lb
void screen15(); //temp ub
void screen16(); //pres lb
void screen17(); //pres ub
void screen18(); //hum lb
void screen19(); //hum ub
//ekrany edycji progów
void screen20(); //temp lb
void screen21(); //temp ub
void screen22(); //pres lb
void screen23(); //pres ub
void screen24(); //hum lb
void screen25(); //hum ub
//ekrany prognozy


void countTimers();
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Wire init, adding the I2C bus.
  M5.begin();
  //setDate();
  qmp6988.init();
  temp_LB = 20; temp_UB = 24;
  hum_LB = 30; hum_UB = 50;
  pres_LB = 950; pres_UB = 1030;
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if(disp_refresh){ M5.Lcd.fillScreen(BLACK); disp_refresh = 0; }

  switch(menu_stan)
  {
  case 1:screen1();break;
  case 2:screen2();break;
  case 3:screen3();break;
  case 4:screen4();break;
  case 5:screen5();break;
  case 6:screen6();break;
  case 7:screen7();break;
  case 8:screen8();break;
  case 9:screen9();break;
  case 10:screen10();break;
  case 11:screen11();break;
  case 12:screen12();break;
  case 13:screen13();break;
  case 14:screen14();break;
  case 15:screen15();break;
  case 16:screen16();break;
  case 17:screen17();break;
  case 18:screen18();break;
  case 19:screen19();break;
  case 20:screen20();break;
  case 21:screen21();break;
  case 22:screen22();break;
  case 23:screen23();break;
  case 24:screen24();break;
  case 25:screen25();break;
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
    else if (M5.BtnB.wasReleased()) {menu_stan = 1; drawScreen=1; timer1 = millis(); timer2=0;}
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
    else if (M5.BtnB.wasReleased()) {menu_stan = 10; drawScreen=1; timer2 = millis();}
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
void screen9()
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
    else if (M5.BtnA.wasPressed()) {menu_stan = 12; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 7; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 10; drawScreen=1; timer2 = millis();}
}
void screen10()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Set date",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 9; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 11; drawScreen=1; timer2 = millis();}
}
void screen11()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Set time",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 10; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 12; drawScreen=1; timer2 = millis();}
}
void screen12()
{
  if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Set",160,60,4);
    M5.Lcd.drawString("bounds",160,120,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 11; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 14; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 9; drawScreen=1; timer2 = millis();}
}
void screen13()
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
    else if (M5.BtnA.wasPressed()) {menu_stan = 19; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 12; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 14; drawScreen=1; timer2 = millis();}
}
void screen14()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("min. T",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 13; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 20; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 15; drawScreen=1; timer2 = millis();}
}
void screen15()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("max. T",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 14; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 21; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 16; drawScreen=1; timer2 = millis();}
}
void screen16()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("min. p",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 15; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 22; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 17; drawScreen=1; timer2 = millis();}
}
void screen17()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("max. p",160,90,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 16; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 23; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 18; drawScreen=1; timer2 = millis();}
}
void screen18()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("min.",160,60,4);
    M5.Lcd.drawString("hum.",160,120,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 17; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 24; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 19; drawScreen=1; timer2 = millis();}
}
void screen19()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("max.",160,60,4);
    M5.Lcd.drawString("hum.",160,120,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 18; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 25; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 13; drawScreen=1; timer2 = millis();}
}
void screen20()
{
  if (drawScreen) {
    drawScreen--;
    char temp_buf[7];
    sprintf(temp_buf,"%.0f", temp_LB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("min. T [C]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(temp_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {temp_LB--;drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 14; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {temp_LB++;drawScreen=1; timer2 = millis();}
}
void screen21()
{
  if (drawScreen) {
    drawScreen--;
    char temp_buf[7];
    sprintf(temp_buf,"%.0f", temp_UB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("max. T [C]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(temp_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {temp_UB--;drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 15; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {temp_UB++;drawScreen=1; timer2 = millis();}
}
void screen22()
{
  if (drawScreen) {
    drawScreen--;
    char pres_buf[4];
    sprintf(pres_buf,"%.0f", pres_LB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("min. p [hPa]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(pres_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {if(pres_LB>=1){pres_LB--;}drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 16; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {pres_LB++;drawScreen=1; timer2 = millis();}
}
void screen23()
{
  if (drawScreen) {
    drawScreen--;
    char pres_buf[4];
    sprintf(pres_buf,"%.0f", pres_UB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("max. p [hPa]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(pres_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {if(pres_UB>=1){pres_UB--;}drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 17; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {pres_UB++;drawScreen=1; timer2 = millis();}
}
void screen24()
{
  if (drawScreen) {
    drawScreen--;
    char hum_buf[4];
    sprintf(hum_buf,"%.0f", hum_LB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("min. hum. [%]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(hum_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {if(hum_LB>=1){hum_LB--;}drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 18; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {if(hum_LB<100){hum_LB++;}drawScreen=1; timer2 = millis();}
}
void screen25()
{
  if (drawScreen) {
    drawScreen--;
    char hum_buf[4];
    sprintf(hum_buf,"%.0f", hum_UB);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("max. hum. [%]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(hum_buf,160,135,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {if(hum_UB>=1){hum_UB--;}drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 19; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {if(hum_UB<100){hum_UB++;}drawScreen=1; timer2 = millis();}
}
