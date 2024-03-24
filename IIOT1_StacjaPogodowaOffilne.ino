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
int timerLimit1 = 5000,timerLimit2 = 8000;
float temp_UB,temp_LB,hum_UB,hum_LB,pres_UB,pres_LB;
bool error;
float getPressure();
float getTemperature();
float getHumidity();
void readSystemDate();
void readSystemTime();
bool checkDate(int y, int m, int d);
void setDate(int y, int m, int d);
void setTime(int h, int m, int s);
int getYear();
int getMonth();
int getDay();
int getHours();
int getMinutes();
int getSeconds();
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
//ekrany ustawień daty
void screen26(); //menu ustawień daty powrót
void screen27(); //dzien
void screen28(); //miesiac
void screen29(); //rok
//ekrany edycji daty
void screen30(); //ustaw dzien
void screen31(); //ustaw miesiac
void screen32(); //ustaw rok
//ekrany ustawień czasu
void screen33(); //menu ustawien czasu powrót
void screen34(); //godziny
void screen35(); //minuty
//ekrany edycji czasu
void screen36(); //ustaw godziny
void screen37(); //ustaw minuty



void countTimers();
void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Wire init, adding the I2C bus.
  M5.begin();
  qmp6988.init();
  temp_LB = 20; temp_UB = 24;
  hum_LB = 30; hum_UB = 50;
  pres_LB = 950; pres_UB = 1030;
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if(disp_refresh){ M5.Lcd.fillScreen(BLACK); disp_refresh = 0; }
  if (menu_stan!=30&&menu_stan!=31&&menu_stan!=32) {error = false;}
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
  case 26:screen26();break;
  case 27:screen27();break;
  case 28:screen28();break;
  case 29:screen29();break;
  case 30:screen30();break;
  case 31:screen31();break;
  case 32:screen32();break;
  case 33:screen33();break;
  case 34:screen34();break;
  case 35:screen35();break;
  case 36:screen36();break;
  case 37:screen37();break;
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

bool checkDate(int y, int m, int d)
{
  if (!(m>=1&&m<=12) || !(y>=1&&y<=9999) || !(d>=1&&d<=31)) return false;
  bool leap = (y % 400 == 0) || (y % 4 == 0 && y % 100 !=0);

  if (m==2)
  {
    if (leap) { if (!(d<=29)) return false; }
    else {if (!(d<=28)) return false;}
  }

  if (m==4 || m==6 || m==9 || m==1) {if (!(d<=30)) return false;}
  return true;
}
  

void setDate(int y, int m, int d) {
  RTCDate.Year = y;  //Set the date.  设置日期
  RTCDate.Month = m;
  RTCDate.Date = d;
  M5.Rtc.SetDate(&RTCDate);}
void setTime(int h, int m, int s)
{
  RTCTime.Hours = h; //Set the time.  设置时间
  RTCTime.Minutes = m;
  RTCTime.Seconds = s;
  M5.Rtc.SetTime(&RTCTime); //and writes the set time to the real time clock.  并将设置的时间写入实时时钟
}
int getYear() {readSystemDate();return RTCDate.Year;}
int getMonth() {readSystemDate();return RTCDate.Month;}
int getDay() {readSystemDate();return RTCDate.Date;}
int getHours() {readSystemTime();return RTCTime.Hours;}
int getMinutes() {readSystemTime();return RTCTime.Minutes;}
int getSeconds() {readSystemTime();return RTCTime.Seconds;}
void countTimers() {timer1 = millis(); timer2 = millis();}

void screen1()
{
  if (drawScreen) {
    drawScreen--;
    int hours = getHours(); int minutes = getMinutes();
    int day = getDay(); int month = getMonth(); int year = getYear();
    char time_buf[10]; char hours_buf[5]; char minutes_buf[5];
    char date_buf[15]; char month_buf[5]; char day_buf[5]; char year_buf[5];
    if (minutes<10) {sprintf(minutes_buf,"0%d", minutes);}
    else {sprintf(minutes_buf,"%d", minutes);}
    if (hours<10) {sprintf(hours_buf,"0%d:", hours);}
    else {sprintf(hours_buf,"%d:", hours);}
    if (day<10) {sprintf(day_buf,"0%d.", day);}
    else {sprintf(day_buf,"%d.", day);}
    if (month<10) {sprintf(month_buf,"0%d.", month);}
    else {sprintf(month_buf,"%d.", month);}
    sprintf(year_buf,"%d",year);
    strcpy(time_buf,hours_buf);strcat(time_buf,minutes_buf);
    strcpy(date_buf,day_buf);strcat(date_buf,month_buf);strcat(date_buf,year_buf);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(time_buf,160,95,4);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawString(date_buf,160,175,4);
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
    char pres_buf[5];
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 9; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 27; drawScreen=1; timer2 = millis();}
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
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 10; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 34; drawScreen=1; timer2 = millis();}
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Min. T",160,90,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Max. T",160,90,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Min. p",160,90,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Max. p",160,90,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Min.",160,60,4);
    M5.Lcd.drawString("hum.",160,120,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Max.",160,60,4);
    M5.Lcd.drawString("hum.",160,120,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Min. T [C]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(temp_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.drawString("Max. T [C]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(temp_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Min. p [hPa]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(pres_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Max. p [hPa]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(pres_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Min. hum. [%]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(hum_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Max. hum. [%]",160,40,4);
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(hum_buf,160,135,4);
    M5.Lcd.setTextSize(1);
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
void screen26()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Return",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 29; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 10; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 27; drawScreen=1; timer2 = millis();}
}
void screen27()
{
    if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Day",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 26; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 30; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 28; drawScreen=1; timer2 = millis();}

  
   
}
void screen28()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Month",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 27; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 31; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 29; drawScreen=1; timer2 = millis();}
}
void screen29()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Year",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 28; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 32; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 26; drawScreen=1; timer2 = millis();}
}
void screen30()
{
  if (drawScreen) {
    drawScreen--;
    char day_buf[4];
    int day = getDay();
    if (day<10) {sprintf(day_buf,"0%d", day);}
    else {sprintf(day_buf,"%d", day);}
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Day",160,40,4);
    if (error) {M5.Lcd.setTextColor(RED);M5.Lcd.drawString("invalid date",160,70,4);M5.Lcd.setTextColor(WHITE);}
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(day_buf,160,135,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {
      int day = getDay(); day--; int month = getMonth(); int year=getYear();
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
    else if (M5.BtnB.wasReleased()) {menu_stan = 27; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {
      int day = getDay(); day++; int month = getMonth(); int year=getYear();
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
}
void screen31()
{
  if (drawScreen) {
    drawScreen--;
    char month_buf[4];
    int month = getMonth();
    if (month<10) {sprintf(month_buf,"0%d", month);}
    else {sprintf(month_buf,"%d", month);}
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Month",160,40,4);
    if (error) {M5.Lcd.setTextColor(RED);M5.Lcd.drawString("invalid date",160,70,4);M5.Lcd.setTextColor(WHITE);}
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(month_buf,160,135,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {
      int day = getDay(); int month = getMonth(); month--; int year=getYear();
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
    else if (M5.BtnB.wasReleased()) {menu_stan = 28; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {
      int day = getDay(); int month = getMonth(); month++; int year=getYear();
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
}
void screen32()
{
  if (drawScreen) {
    drawScreen--;
    char year_buf[5];
    int year = getYear();
    sprintf(year_buf,"%d", year);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Year",160,40,4);
    if (error) {M5.Lcd.setTextColor(RED);M5.Lcd.drawString("invalid date",160,70,4);M5.Lcd.setTextColor(WHITE);}
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(year_buf,160,135,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {
      int day = getDay(); int month = getMonth(); int year=getYear(); year--;
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
    else if (M5.BtnB.wasReleased()) {menu_stan = 29; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {
      int day = getDay(); int month = getMonth(); int year=getYear(); year++;
      if (checkDate(year,month,day)) {setDate(year,month,day); error=false;}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
}
void screen33()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Return",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 35; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 11; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 34; drawScreen=1; timer2 = millis();}
}
void screen34()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Hours",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 33; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 36; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 35; drawScreen=1; timer2 = millis();}
}
void screen35()
{
   if (drawScreen) {
    drawScreen--;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(3);
    M5.Lcd.drawString("Minutes",160,90,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("<",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString(">",275,240,4);
    }
    if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {menu_stan = 34; drawScreen=1; timer2 = millis();}
    else if (M5.BtnB.wasReleased()) {menu_stan = 37; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {menu_stan = 33; drawScreen=1; timer2 = millis();}
}
void screen36()
{
  if (drawScreen) {
    drawScreen--;
    char hours_buf[4];
    int hours = getHours();
    if (hours<10) {sprintf(hours_buf,"0%d", hours);}
    else {sprintf(hours_buf,"%d", hours);}
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Hours",160,40,4);
    if (error) {M5.Lcd.setTextColor(RED);M5.Lcd.drawString("invalid time",160,70,4);M5.Lcd.setTextColor(WHITE);}
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(hours_buf,160,135,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {
      int hours = getHours(); hours--;
      if (hours>=0&&hours<=23) {setTime(hours,getMinutes(),getSeconds());}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
    else if (M5.BtnB.wasReleased()) {menu_stan = 34; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {
      int hours = getHours(); hours++;
      if (hours>=0&&hours<=23) {setTime(hours,getMinutes(),getSeconds());}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
}
void screen37()
{
  if (drawScreen) {
    drawScreen--;
    char minutes_buf[4];
    int minutes = getMinutes();
    if (minutes<10) {sprintf(minutes_buf,"0%d", minutes);}
    else {sprintf(minutes_buf,"%d", minutes);}
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Minutes",160,40,4);
    if (error) {M5.Lcd.setTextColor(RED);M5.Lcd.drawString("invalid time",160,70,4);M5.Lcd.setTextColor(WHITE);}
    M5.Lcd.setTextSize(4);
    M5.Lcd.drawString(minutes_buf,160,135,4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextDatum(BL_DATUM);
    M5.Lcd.drawString("-",45,240,4);
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.drawString("OK",160,240,4);
    M5.Lcd.setTextDatum(BR_DATUM);
    M5.Lcd.drawString("+",275,240,4);
    }
  if ((millis()-timer2>=timerLimit2)||(millis()-timer2<0)) {menu_stan = 1; drawScreen=1; timer1=millis();timer2=0;}
    else if (M5.BtnA.wasPressed()) {
      int minutes = getMinutes(); minutes--;
      if (minutes>=0&&minutes<=59) {setTime(getHours(),minutes,getSeconds());}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
    else if (M5.BtnB.wasReleased()) {menu_stan = 35; drawScreen=1; timer2 = millis();}
    else if (M5.BtnC.wasPressed()) {
       int minutes = getMinutes(); minutes++;
      if (minutes>=0&&minutes<=59) {setTime(getHours(),minutes,getSeconds());}
      else {error=true;}
      drawScreen=1; timer2 = millis();
      }
}
