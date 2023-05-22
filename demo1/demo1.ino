#include "Arduino.h"
#include "src/oled12864.h"

#define U_N 14
#define U_P 27
#define V_N 26
#define V_P 25
#define W_N 33
#define W_P 32

#define EN_C 19

void motomove(int step)
{
  switch(step)
  {
    case 0:
    digitalWrite(U_N, 1);
    digitalWrite(U_P, 0);
    digitalWrite(V_N, 1);
    digitalWrite(V_P, 0);
    digitalWrite(W_N, 1);
    digitalWrite(W_P, 0);
    break;
    case 1:
    digitalWrite(U_N, 0);
    digitalWrite(U_P, 0);
    digitalWrite(V_N, 1);
    digitalWrite(V_P, 0);
    digitalWrite(W_N, 1);
    digitalWrite(W_P, 1);
    break;
    case 2:
    digitalWrite(U_N, 1);
    digitalWrite(U_P, 0);
    digitalWrite(V_N, 1);
    digitalWrite(V_P, 1);
    digitalWrite(W_N, 1);
    digitalWrite(W_P, 0);
    break;
    case 3:
    digitalWrite(U_N, 1);
    digitalWrite(U_P, 1);
    digitalWrite(V_N, 1);
    digitalWrite(V_P, 0);
    digitalWrite(W_N, 0);
    digitalWrite(W_P, 0);
    break;
    case 4:
    digitalWrite(U_N, 1);
    digitalWrite(U_P, 0);
    digitalWrite(V_N, 0);
    digitalWrite(V_P, 0);
    digitalWrite(W_N, 1);
    digitalWrite(W_P, 0);
    break;

  }
}

void setup() {
  // put your setup code here, to run once:

//moto pin mode
  pinMode(U_N, OUTPUT);
  pinMode(U_P, OUTPUT);
  pinMode(V_N, OUTPUT);
  pinMode(V_P, OUTPUT);
  pinMode(W_N, OUTPUT);
  pinMode(W_P, OUTPUT);
//encode pin mode
  pinMode(EN_C,INPUT_PULLUP);


  digitalWrite(U_N, 1);
  digitalWrite(U_P, 0);
  digitalWrite(V_N, 1);
  digitalWrite(V_P, 0);
  digitalWrite(W_N, 1);
  digitalWrite(W_P, 0);
  

  Serial.begin(115200);
  Wire.begin();
  OLED_Init();

  OLED_Str(0,0,16,"test",0,1);
}

int ii=0;
char srt[64];
void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(EN_C)==0)
  {
    OLED_Str(0,0,16,"push",0,1);
    ii+=1;
    if(ii==5){ii=0;}
  }else
  {
    OLED_Str(0,0,16,"test",0,1);
    ii=0;
  }
  sprintf(srt,"index:%d",ii);
  OLED_Str(0,16,16,srt,0,1);
  motomove(ii);
  delay(10);
  
  OLED_Cache_to_hardware();
  //OLED_Init();

}
