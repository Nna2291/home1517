#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <DHT.h>
#define DHTPIN 5

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "sch1517";       // имя вашей wifi точки доступа
const char *password = "Gfhjvyfz "; // пароль wifi
const IPAddress host(172, 16, 100, 240);
const int httpPort = 5000;
String hum;
String temps;

float return_th()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  return temperature, humidity;
}

void setup()
{
  Serial.begin(115200);
  delay(10);
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
  delay(1000);
  WiFiClient client;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  float f = dht.readTemperature(true);
  if (isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.println(humidity);
  Serial.println(temperature);
  client.print(String("GET /dick?hum=" + String(humidity) + "&jio=" + String(temperature)) + "\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
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
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, c.substring(139));
  Serial.println(c.substring(139));
  Serial.println();
  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write("Hello Slave"); /* sends hello string */
  Wire.endTransmission();    /* stop transmitting */
  Wire.requestFrom(8, 15);   /* request & read data of size 9 from slave */
  String jo;
  while (Wire.available())
  {
    char j = Wire.read(); /* read data received from slave */
    jo += j;
  }
  Serial.println(jo);
}