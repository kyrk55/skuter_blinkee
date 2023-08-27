#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include "src/ESP2SOTA/src/ESP2SOTA.h"
#include "src/DS3231/DS3231.h"
#include "src/PCF8575/PCF8575.h"
//#include "src/TFT_eSPI/TFT_eSPI.h"
#include <WiFiAP.h>
#include "lodomir.h"

#include "Final_Frontier288.h"
TwoWire I2Ctwo = TwoWire(1);
PCF8575 pcf8575(&I2Ctwo,0x20,21,22);
DS3231 RTC;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite buf = TFT_eSprite(&tft);
TFT_eSprite arrow = TFT_eSprite(&tft);
TFT_eSprite arrowS = TFT_eSprite(&tft);

char AP_SSID[32] = "blinkee";
#define AP_PASS "12345678"

IPAddress Actual_IP;

IPAddress PageIP(192,168,1,1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

WebServer server(80);


  String test;
//////////////////////////////////////////////////////////
//odczyt danych
long long_time;
long tt;
bool kk;
bool error;
int counter;

bool words[97] ;//= {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool words2[97] = {};
char gears[4] = {'P','3','2','1'};
void odczyt() {
  kk = !digitalRead(23);
  tt = micros() - long_time;
  if(kk == 0 && tt > 30000){
    if(error == 0){
      for(int i = 0; i<97; i++){
        words[i] = words2[i];
      }
    }
    counter = 0;
    error = 0;
  }
  if(kk == 1 && counter < 97){
    if(tt < 600){
      words2[counter] = 0;
    }
    else{
      words2[counter] = 1;
    }
    counter++;
  }
  else if(kk == 1){
    error = 1;
  }
  long_time = micros();
}

int ReadFromTable(int poczatek, int koniec){
  int temp = 0;
  int dlugosc = (poczatek - koniec);
  if(dlugosc > 0){
    for(int i = 0; i<=abs(dlugosc); i++){
      temp = temp + (words[poczatek-i] << i);
    }
  }
  else if(dlugosc < 0){
    for(int i = 0; i<=abs(dlugosc); i++){
      temp = temp + (words[koniec+i] << i);
    }
  }
  else temp = words[koniec];
  return temp;
}
//////////////////////////////////////////////////////////

  
void setup() {
  attachInterrupt(digitalPinToInterrupt(23), odczyt, CHANGE);
  
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  ESP2SOTA.begin(&server);
  server.begin();
  Serial.begin(115200);

  tft.init(); 
  tft.setSwapBytes(true);
  tft.setRotation(1);
  tft.fillScreen(rgb(0,0,0));
  //tft.pushImage(0,0,800,480,lodomir,TFT_BLACK);
  //tft.fillEllipse(285, 285, 10, 10, TFT_WHITE);
//  buf.createSprite(230, 230);
//  buf.setSwapBytes(true);
//  arrow.createSprite(230, 11);
//  arrow.setSwapBytes(true);
//  arrow.fillScreen(rgb(255,0,0));
//  arrow.setPivot(230, 6);
//  arrowS.createSprite(100, 5);
//  arrowS.setSwapBytes(true);
//  arrowS.fillScreen(rgb(255,0,0));
//  arrowS.setPivot(100, 3);
  pinMode(19,OUTPUT);
  digitalWrite(19,1);
  //Wire.begin();
  pcf8575.begin();
  pcf8575.pinMode(P0, INPUT);
  pcf8575.pinMode(P1, INPUT);
  pcf8575.pinMode(P2, INPUT);
  pcf8575.pinMode(P3, INPUT);
  pcf8575.pinMode(P4, INPUT);
  pcf8575.pinMode(P5, INPUT);
  pcf8575.pinMode(P15, OUTPUT);
//  pcf8575.setSecond(0);
//  pcf8575.setMinute(17);
//  pcf8575.setHour(17);
}
int x = 0;
int delta=0;
bool life;
int stopnie0 = 0;
float stopnie = 0;
float stopnie1 = 0;
float stopnie2 = 0;
int pen;
double odo=7054;
void loop() {
  life = !life;
  //pcf8575.digitalWrite(P15, life);

//  if(life) stopnie += 1; //map(speeds,10,70,0,180);
//  else stopnie -= 1;
//  if(stopnie > 80) life = false;
//  else if(stopnie < 0) life = true;




  //drawG(stopnie,stopnie1,stopnie2,(millis()/1000)&0b11111111,millis()/1000,pen,"02:26");
  //sipleDraw("test1","test2","test3");
  String helper;
  tft.setTextDatum(TR_DATUM);
  tft.setTextSize(2);
  helper = "[";
  for(int i = 0; i<33; i++){
    helper += words[i];
  }
  helper += "]";
  tft.drawString(helper,800,0,1);
  
  helper = "[";
  for(int i = 33; i<65; i++){
    helper += words[i];
  }
  helper += "]";
  
  tft.drawString(helper,800,26,1);
  helper = "[";
  for(int i = 65; i<97; i++){
    helper += words[i];
  }
  helper += "]";
  tft.drawString(helper,800,52,1);
  helper = "[" + String(pcf8575.digitalRead(P0)) + "," + String(pcf8575.digitalRead(P1)) + "," + String(pcf8575.digitalRead(P2)) + ","+String(pcf8575.digitalRead(P3)) + ","+String(pcf8575.digitalRead(P4)) + ","+String(pcf8575.digitalRead(P5)) + "]";
//  tft.drawString(String(life),800,360,1);
  tft.drawString(helper,800,78,1);
 
  tft.setTextSize(8);  
  tft.drawString("          "+String(ReadFromTable(72,57)),620,220,6);
  tft.drawString("km/h",800,220,1);
  tft.drawString(String(gears[ReadFromTable(40,39)]),750,300,4);
  tft.drawString("   "+String(ReadFromTable(56,48))+"A",800,100,2);
  

  tft.setTextDatum(TL_DATUM);
  tft.drawString(getTime(),0,0,1);
  tft.drawString(String(74.6,1)+"V    ",0,54,1); 
  tft.drawString("ODO:"+String(odo,0)+"km ",0,100,2);
  delta = millis() - delta;
  pen = delta;
  odo += (float(ReadFromTable(72,57))/3600.0)*(float(pen)/1000.0);
  delta = millis();
  server.handleClient();
}

String getTime(){
  int h = pcf8575.getHour();
  int m = pcf8575.getMinute();
  int s = pcf8575.getSecond();
  String out;
  if(h < 10) out += "0";
  out += String(h);
  out += ":";
  if(m < 10) out += "0";
  out += String(m);
  out += ":";
  if(s < 10) out += "0";
  out += String(s);
  return out;
}

int pid(float curent, float to, float k = 10){
  if(curent == to) return(curent);
  else{
    int dif = (to - curent)/k; 
    if(dif > 20) dif = 20;
    if(curent - to < 1 && !dif) dif = 1;
    if(curent - to > 1 && !dif) dif = -1;
    return curent+dif;
  }
}

int zegarySoft[6] = {-31,89,-31,0,0,0};
String oldTime;

void drawG(float spe, float amp, float soc, uint8_t controls, int odo,int trip, String times){
  int k=0;
  uint8_t onoff = 0;
  int stopnie = map(spe*10,0,800,-30,210);
  if(zegarySoft[0] != stopnie || zegarySoft[3] != controls ){
    onoff += 0b1;
    k+=2;
  }
  if(zegarySoft[0] != stopnie || zegarySoft[4] != odo || zegarySoft[5] != trip) {
    onoff += 0b10;
    k+=2;
  }
  stopnie = map(amp*10,-1000,1000,-30,210);
  if(zegarySoft[1] != stopnie || oldTime != times) {
    onoff += 0b100;
    k++;
  }
  stopnie = map(soc*10,0,1000,-30,210);
  if(zegarySoft[2] != stopnie) {
    onoff += 0b1000;
    k++;
  }
  k=(5*7)-(k*5);
  
  Serial.print("asd");
  Serial.println(k);
  if(onoff & 0b1){
    stopnie = map(spe*10,0,800,-30,210);
    zegarySoft[0] = pid(zegarySoft[0],stopnie,k);
    zegarySoft[3] = controls;

        buf.pushImage(-285,-55,800,480,lodomir);
        if(controls&0b00001000) buf.pushImage(0,62,65,62,breaks);
        if(controls&0b00000100) buf.pushImage(0,124,65,62,low);
        if(controls&0b00000010) buf.pushImage(65,62,65,62,kierunek);
        if(controls&0b00000001) buf.pushImage(65,124,65,62,eco);
        buf.setPivot(0, 230);
        arrow.pushRotated(&buf,zegarySoft[0]);
        buf.fillEllipse(0, 230, 10, 10, TFT_WHITE);
        buf.pushSprite(285, 55);


        buf.pushImage(-55,-55,800,480,lodomir);
        if(controls&0b10000000) buf.pushImage(230-130,62,65,62,kierunek);
        if(controls&0b01000000) buf.pushImage(230-130,124,65,62,check);
        if(controls&0b00100000) buf.pushImage(230-65,62,65,62,high);
        if(controls&0b00010000) buf.pushImage(230-65,124,65,62,temp);
        buf.setPivot(230, 230);
        arrow.pushRotated(&buf,zegarySoft[0]);
        buf.fillEllipse(230, 230, 10, 10, TFT_WHITE);
        buf.pushSprite(55, 55);
  }
  if(onoff & 0b10){
    stopnie = map(spe*10,0,800,-30,210);
    zegarySoft[0] = pid(zegarySoft[0],stopnie,k);
    zegarySoft[4] = odo;
    zegarySoft[5] = trip;
    
        buf.pushImage(-285,-285,800,480,lodomir);
        buf.setPivot(0, 0);
        arrow.pushRotated(&buf,zegarySoft[0]);
        buf.fillEllipse(0, 0, 10, 10, TFT_WHITE);
        buf.setTextDatum(TC_DATUM);
        buf.setTextColor(TFT_BLACK,TFT_WHITE);  buf.setTextSize(2);
        buf.setCursor(0, 100, 2);
        buf.drawString(String(odo) + " ODO",0,100,2);
        buf.setCursor(0, 130, 2);
        buf.drawString(String(trip) + " TRIP",0,130,2);
        buf.pushSprite(285, 285);

        buf.pushImage(-55,-285,800,480,lodomir);
        buf.setPivot(230, 0);
        arrow.pushRotated(&buf,zegarySoft[0]);
        buf.fillEllipse(230, 0, 10, 10, TFT_WHITE);  
        buf.setTextDatum(TC_DATUM);
        buf.setTextColor(TFT_BLACK,TFT_WHITE);  buf.setTextSize(2);
        buf.setCursor(230, 100, 2);
        buf.drawString(String(odo) + " ODO",230,100,2);
        buf.setCursor(230, 130, 2);
        buf.drawString(String(trip) + " TRIP",230,130,2);
        buf.pushSprite(55, 285);
  }
  

  
  if(onoff & 0b100){
    stopnie = map(amp*10,-1000,1000,-30,210);
    zegarySoft[1] = pid(zegarySoft[1],stopnie,k);
    oldTime = times;
    
    buf.pushImage(-560,-15,800,480,lodomir);
    buf.setPivot(115, 115);
    arrowS.pushRotated(&buf,zegarySoft[1]);
    buf.fillEllipse(115, 115, 5, 5, TFT_WHITE);
    buf.pushSprite(560, 15);
    
    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(2);
    tft.setCursor(674, 225, 2);
    tft.drawString(times,675,220,2);
  }
  
  if(onoff & 0b1000){
    stopnie = map(soc*10,0,1000,-30,210);
    zegarySoft[2] = pid(zegarySoft[2],stopnie,k);
    buf.pushImage(-560,-265,800,480,lodomir);
    buf.setPivot(115, 115);
    arrowS.pushRotated(&buf,zegarySoft[2]);
    buf.fillEllipse(115, 115, 5, 5, TFT_WHITE);
    buf.pushSprite(560, 265);
  }
}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
    if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
        return -1;

    int result;

    int red = r * 31 / 255;
    int green = g * 63/ 255;
    int blue = b * 31 / 255;


    //int result = (red << 11) | (green << 5) | blue;

    green = green << 5;
    red = red << 11;

    result = red | green | blue;


    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}
