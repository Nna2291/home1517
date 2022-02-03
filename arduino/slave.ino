#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
String data;

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);           /* start serial comm. */
  Serial.println("I am I2C Slave");
  dht.begin();
}

void loop() {

}

void receiveEvent(int howMany) {
  String jo;
  while (0 < Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    jo += c;           /* print the character */
  }
  Serial.println(jo);             /* to newline */
}
// function that executes whenever data is requested from master
void requestEvent() {
  float temp, hum;
  temp, hum = check_dht();
  data = String(hum);
  Wire.write(data.c_str());  /*send string on request */
}

float check_dht() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  return temperature, humidity;
}
