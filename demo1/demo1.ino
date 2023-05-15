#include "Arduino.h"
#include "src/oled12864.h"

#define U_N 14
#define U_P 27
#define V_N 26
#define V_P 25
#define W_N 33
#define W_P 32


void setup() {
  // put your setup code here, to run once:
  pinMode(U_N, OUTPUT);
  pinMode(U_P, OUTPUT);
  pinMode(V_N, OUTPUT);
  pinMode(V_P, OUTPUT);
  pinMode(W_N, OUTPUT);
  pinMode(W_P, OUTPUT);


  digitalWrite(U_N, 1);
  digitalWrite(U_P, 0);
  digitalWrite(V_N, 1);
  digitalWrite(V_P, 0);
  digitalWrite(W_N, 1);
  digitalWrite(W_P, 0);
  
  Serial.begin(115200);
  Wire.begin();
  OLED_Init();

}

void loop() {
  // put your main code here, to run repeatedly:

  OLED_Str(0,0,16,"test",0,1);
  OLED_Cache_to_hardware();
  //OLED_Init();

}
