#include <ESP8266WiFi.h>

const char* ssid = "ITP1"; // имя вашей wifi точки доступа
const char* password = "Gfhjvyfz_"; // пароль wifi
const IPAddress host(172, 16, 102, 236);
const int httpPort = 5000;


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
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  delay(5000);
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  //  Serial.println("???");
  //  Serial.println(digitalRead);
  //  Serial.println("???");
  // запрос к серверу
  client.print(String("GET / ") + "HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long timeout = millis();

  while (client.available() == 0) // ждёт ответ от сервера
  {
    if (millis() - timeout > 5000) // если нет ответа в течении 5 сек, то разрывает соединение
    {
      Serial.println("Client Timeout");
      client.stop();
      return;
    }
  }

  while (client.available()) // читает построчно ответ от сервера
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if (line == "hi" and line != "") {
      digitalWrite(LED_BUILTIN, HIGH);
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      }
  }

  Serial.println();
  Serial.println("closing connection");
}
