#include <Wire.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3

int r_data = 0;
int g_data = 0;
int b_data = 0;
int c = 0;
byte dataArray[4];
void setup() {
  Wire.begin(8);
  Wire.onReceive(lights);
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
}


void lights() {
  for (int i = 0; i < 4; i++)
  {
    dataArray[i] = Wire.read();
    Serial.print(dataArray[i]);
  }
  r_data = dataArray[0];
  b_data = dataArray[1];
  g_data = dataArray[2];
  c = dataArray[3];
  Serial.println();
}