#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

int motorPin = 12;

int auto_light = 0;
int light_val = 0;
int light_status = 0;
int vent_status = 0;

const char *ssid = "sch1517";       // имя вашей wifi точки доступа
const char *password = "Gfhjvyfz "; // пароль wifi
const IPAddress host(172, 16, 101, 33);
const int httpPort = 5000;
DynamicJsonDocument doc(1024);

void setup()
{
  Wire.begin();

  pinMode(motorPin, OUTPUT);
  pinMode(A0, INPUT);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) // подключение к точке
  {
    delay(500);
  }
}


void loop()
{
  String c;
  delay(500);
  WiFiClient client;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET /second_floor?light_val=" + String(light_val) + "\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"));
  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 1000)
    {
      Serial.println("Client Timeout");
      client.stop();
      return;
    }
  }

  while (client.available())
  {
    String line = client.readStringUntil('\r');
    c += line;
  }
  deserializeJson(doc, c);
  int red = doc["red"];
  int green = doc["green"];
  int blue = doc["blue"];
  int auto_light = doc["auto_light"];
  int light_status = doc["light_status"];
  int vent_status = doc["vent"];

  vent(vent_status);

  lights(red, green, blue, light_status, auto_light);
}

void vent(int v_s)
{
  if (v_s == 1)
  {
    digitalWrite(12, HIGH);
  }
  else
  {
    digitalWrite(12, LOW);
  }
}


void lights(int r, int g, int b, int l_s, int a_l)
{
  light_val = analogRead(A0);

  if (a_l == 1)
  {
    if (light_val < 600) {l_s = 0;}
    else {l_s = 1;}
  }
      Wire.beginTransmission(8);
      Wire.write(r);
      Wire.write(g);
      Wire.write(b);
      Wire.write(l_s);
      Wire.endTransmission();
}