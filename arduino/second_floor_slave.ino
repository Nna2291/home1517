#include <Wire.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define MOTORPIN 12
#define HEATPIN 8

int r_data = 0;
int g_data = 0;
int b_data = 0;
int c = 0;
int v_s = 0;
int h_s = 0;
byte dataArray[6];

void setup() {
  Wire.begin(8);
  Wire.onReceive(to_slave);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(HEATPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  if (c == 1) {
    analogWrite(REDPIN, 255 - r_data);
    analogWrite(GREENPIN, 255 - g_data);
    analogWrite(BLUEPIN, 255 - b_data);
  }
  else {
    analogWrite(REDPIN, 255);
    analogWrite(GREENPIN, 255);
    analogWrite(BLUEPIN, 255);
  }
  if (v_s == 1){
    digitalWrite(MOTORPIN, HIGH);
  }
  else{
    digitalWrite(MOTORPIN, LOW);
  }
  if (h_s == 1){
    digitalWrite(HEATPIN, HIGH);
  }
  else{
    digitalWrite(HEATPIN, LOW);
  }
}


void to_slave() {
  for (int i = 0; i < 6; i++)
  {
    dataArray[i] = Wire.read();
    Serial.print(dataArray[i]);
  }
  r_data = dataArray[0];
  b_data = dataArray[1];
  g_data = dataArray[2];
  c = dataArray[3];
  v_s = dataArray[4];
  h_s = dataArray[5];
  Serial.println();
}