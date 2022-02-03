#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
String data;
byte data1[10];

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendRqst);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temp;
  float hum;
  temp, hum = check_dht();
  data = String(hum);
  data.getBytes(data1, sizeof(data1));
  Serial.println(data.substring(0, 2));
}

void sendRqst() {
  Wire.write(data1, 10);
}

void receiveEvent(){
  String ans;
  while (0 < Wire.available()){
    char c = Wire.read();
    ans += c;
  }
  Serial.println(ans);
}



float check_dht() {
  delay(200);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  return temperature, humidity;
}