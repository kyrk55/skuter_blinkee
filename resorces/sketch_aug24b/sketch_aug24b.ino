bool words[97] = {};
bool words2[97] = {};
//bool words2[97] = {1,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,0,1,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0};
long long_time;
long tt;
bool kk;
bool error;
int counter;

float curent=80;
float rpm=100;
float speeds=55;
int spe=0;


void test() {
  kk = !digitalRead(2);
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

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), test, CHANGE);
  Serial.begin(250000);
  clearterm();
  cursorshow(0);
}

void loop() {
  int temp = 0;
  for(int i = 0; i<16; i++){
    temp = temp + (words[72-i] << i);
  }
  rpm = int(temp);
  speeds = map(temp,0,2000,0,1600);
  speeds = speeds/100;
  
  temp = 0;
  for(int i = 0; i<8; i++){
    temp = temp + (words[57-i] << i);
  }
  
  curent = map(temp,0,20000,0,6000);
  curent = curent/100;

  temp = 0;
  for(int i = 0; i<2; i++){
    temp = temp + (words[40-i] << i);
  }
  
  spe = 4-temp;
    
  setcursor(2,2);
  textcolor(9);
  backtext(0);
  Serial.print("Curent: ");
  Serial.print(curent);
  Serial.print(" A    ");
  
  setcursor(3,2);
  textcolor(9);
  backtext(0);
  Serial.print("Speed:  ");
  Serial.print(speeds);
  Serial.print(" km/h     ");
  
  setcursor(4,2);
  textcolor(9);
  backtext(0);
  Serial.print("RPM:    ");
  Serial.print(rpm);
  Serial.print("     ");

  setcursor(5,2);
  textcolor(9);
  backtext(0);
  Serial.print("3-speed: ");
  Serial.print(spe);
  Serial.print("     ");
    
  setcursor(21,2);
  textcolor(9);
  backtext(0);
  for(int i = 0; i<97; i++){
    if(i/10 > 0)Serial.print(String(i/10));
    else Serial.print(" ");
  }
  setcursor(22,2);
  textcolor(9);
  backtext(0);
  for(int i = 0; i<97; i++){
    Serial.print(String(i%10));
  }
  setcursor(23,0);
  textcolor(9);
  backtext(0);
  Serial.print("[");
  for(int i = 0; i<97; i++){
    textcolor(0);
    if(words[i] == 1) backtext(2);
    else backtext(1);
    Serial.print(words[i]);
  }
  textcolor(9);
  backtext(0);
  Serial.println("]");
  
  delay(100);
  backtext(0);
  //clearterm();
}

void include(void){
  Serial.write(0x1B);
  Serial.write('[');
}
void textcolor(int c){
  include();
  Serial.print("3");
  Serial.print(c);
  Serial.print("m");
}
void backtext(int c){
  include();
  Serial.print("4");
  Serial.print(c);
  Serial.print("m");
}
void backterm(int c){
  backtext(c);
  clearterm();
}
void setcursor(int y,int x){
  include();
  Serial.print(y);
  Serial.print(";");
  Serial.print(x);
  Serial.print("H");
}
void clearterm(void){
  include();
  Serial.print("2J");
  Serial.write(0x1B);
  Serial.write('[');
  Serial.print("H");
}
void cursorshow(boolean cur){
  include();
  if(cur == true){
    Serial.print("?25h");}
  else{
    Serial.print("?25l");}
}
