#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>

const char *ssid = "sch1517";       // имя вашей wifi точки доступа
const char *password = "Gfhjvyfz "; // пароль wifi
const IPAddress host(172, 16, 103, 1);
const int httpPort = 5000;
DynamicJsonDocument doc(1024);

int auto_light = 0;
int light_val = 0;
int light_status = 0;


void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Wire.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) // подключение к точке
  {
    delay(500);
  }

}

void loop() {
  String c;
  WiFiClient client;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET /first_floor" + "\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"));

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
  bool new_data = true;
  while (client.available() && new_data)
  {
    String line = client.readStringUntil('}');
    c += line;
    c += '}';
    new_data = false;
  }
  String a = c.substring(c.indexOf('{'));
  deserializeJson(doc, a);
  int red = doc["red1"];
  int green = doc["green1"];
  int blue = doc["blue1"];
  int auto_light = doc["auto_light1"];
  int light_status = doc["light_status1"];
  int light_val = doc["light_val"];

  lights(red, green, blue, light_status, auto_light);
}


void lights(int r, int g, int b, int l_s, int a_l, int l_v)
{
  if (a_l == 1)
  {
    if (l_v < 750) {
      l_s = 0;
    }
    else {
      l_s = 1;
    }
  }
  Wire.beginTransmission(8);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.write(l_s);
  Wire.endTransmission();
}