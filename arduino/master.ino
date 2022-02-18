#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <DHT.h>
#include <Servo.h>

Servo myservo;
#define DHTPIN 5

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "ITP1";       // имя вашей wifi точки доступа
const char *password = "Gfhjvyfz_"; // пароль wifi
const IPAddress host(172, 16, 103, 1);
const int httpPort = 5000;
String humidity = "";
String temperature = "";
DynamicJsonDocument doc(1024);

String return_th()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  return String(temperature), String(humidity);
}

void setup()
{
  Serial.begin(115200);
  myservo.attach(13);
  myservo.write(0);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) // подключение к точке
  {
    delay(500);
  }
  dht.begin();
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
  client.print(String("GET /dick2?hum=" + humidity + "&temp=" + temperature + "\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"));

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
  int a = doc["door"];
  Serial.println(a);
  if (a == 1)
  {
    myservo.write(180);
  }
  else
  {
    myservo.write(0);
  }
}