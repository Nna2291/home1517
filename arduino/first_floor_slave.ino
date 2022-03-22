#include <Adafruit_Sensor.h>
#include <Servo.h>
#include <Wire.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3


const int FSR_PIN = A0; // Pin connected to FSR/resistor divider
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 220.0; // Measured resistance of 3.3k resistor
const int PIN_BUTTON = 2;


int r_data = 0;
int g_data = 0;
int b_data = 0;
int c = 0;
byte dataArray[4];


int redPin = 12;
int greenPin = 8;
int bluePin = 13;


boolean first_start = true; // Первый старт
int mode = 0; // Первое нажатие

boolean btn = false; // Обработчик кнопки для цикла.
int fsr = analogRead(FSR_PIN);
boolean fsrt = false;

//КНОПКА
int mode_w = 0; // Обработчик времени между нажатиями
int mode_t = 0; // Обработчик во время нажатия в нужное время.

//ДАТЧИК СИЛЫ
int mode_e = 0;
int mode_g = 0;

int next = 0; //Разрешение на проход на следующий этап.

int age = 1; //Какой этап должен происходить
int era = 1;

Servo servo;
Servo lock;


void setup() {
  Serial.begin(9600);
  // Servo-часть
  servo.attach(10);
  lock.attach(11);


  lock.write(0); //закрыть. У серво 0 - открыто. У лока 0 - закрыто.

  
  pinMode(FSR_PIN, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);


  //Wire.onReceive(lights);
  Wire.begin(8);


  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  datas();

  
  if (first_start){
    int fsrADC = analogRead(FSR_PIN);
    bool btnState = !digitalRead(PIN_BUTTON);
    if (fsrADC > 60) {
      mode = 1;
      first_start = false;
      Serial.println("Выбор датчика силы");
      delay(500);
    }
    
    else if (btnState) {
      mode = 2;
      first_start = false;
      Serial.println("Выбор кнопки");
      delay(500);
    }
}
  // Заверешние обработки первого нажатия, начало обработки событий


  //Если первый датчик силы
  else if (mode == 1){
    if (era == 1){
    wait_power(3);
    if (next){
      next = 0;
      wait_power(5);
      }
    if (next){
      next = 0;
      Serial.println("Удачная отработка! x3 . Переход на 2 эру...");
      era = 2;
      }
    }
  if (era == 2){
    wait_button(4);
    if (next){
      next = 0;
      wait_button(4);
      }
    if (next){
      next = 0;
      wait_button(2);
      }
    if (next){
      //Переход на 2 эру.
      //Здесь будет код с полным механизмом открытия двери.
      Serial.println("Успешно! Обе эры завершены!");
      go(servo , lock);
      first_start = true;
      mode = 0;
      next = 0;
      era = 1;
      }
    }
  }

  //Если первая кнопка
  else if (mode == 2){
    if (era == 1){
    wait_button(4);
    if (next){
      next = 0;
      wait_button(2);
      }
    if (next){
      next = 0;
      Serial.println("Успешная обработка! Переход на 2 эру...");
      era = 2;
      }
    }
   if (era == 2){
       wait_power(4);
    if (next){
      next = 0;
      wait_power(3);
      }
    if (next){
      next = 0;
      wait_power(5);
      }
    if (next){
      Serial.println("Успешно! Обе эры завершены!");
      go(servo , lock);
      first_start = true;
      mode = 0;
      next = 0;
      era = 1;
    }
    }
  }
}

void yield(){
  boolean btnState = !digitalRead(PIN_BUTTON);
  int fsrADC = analogRead(FSR_PIN);
  if (mode_w == 1){
    if (btnState){
      era = 1;
      mode = 0;
      next = 1;
      btn = true;
      first_start = true;
      }
    }
    
  else if (mode_t == 1){
    if (btnState){
      btn = true;
      next = 1;
      }
    }

  else if (mode_e == 1){
    if (fsrADC > 40){
      era = 1;
      mode = 0;
      next = 1;
      fsrt = true;
      first_start = true;
      }
    }

  else if (mode_g == 1){
    if (fsrADC > 40){
    fsrt = true;
    next = 1;
    }
    }
  }


//Метод для обработки нажатий на кнопку.
void wait_button(int times){
  int count = 0; // Счётчик секунд
  btn = !digitalRead(PIN_BUTTON);
  while (!btn){
    count += 1;
    Serial.println(count);
    mode_w = 1;
    delay(1000);
    mode_w = 0;
    if (next){
      Serial.println("Ошибка");
      era = 1;
      next = 0;
      }
    else if (count == times){
      Serial.println("Время жать");
      mode_t = 1;
      delay(1000);
      mode_t = 0;
      if (!next){
        Serial.println("Нажатие не свершилось!");
        era = 1;
        btn = true;
        first_start = true;
        }
      }
    }
  }


//Метод для обработки ударов по датчику силы.
void wait_power(int times){
  int count = 0; //Счётчик секунд
  fsr = analogRead(FSR_PIN);
  if (fsr > 40){
    fsrt = true;
    }
  else{
    fsrt = false;
    }
  
  while (!fsrt){
    count += 1;
    Serial.println(count);
    mode_e = 1;
    delay(1000);
    mode_e = 0;
    if (next){
      Serial.println("Ошибка");
      era = 1;
      next = 0;
      }
    else if (count == times){
      Serial.println("Время жать");
      mode_g = 1;
      delay(1000);
      mode_g = 0;
      if (!next){
        Serial.println("Нажатие не свершилось!");
        era = 1;
        fsrt = true;
        first_start = true;
        }
      }
    }
  }






void go(Servo servo, Servo lock) {
    for (int i = 10; i < 91; i += 10) {
    lock.write(i);
    delay(100);
  }
  delay(400);
  servo.write(0); // открыть дверь
  delay(7000);
  servo.write(180);
  delay(400);
  for (int i = 80; i > -1; i -= 10) {
    lock.write(i);
    delay(100);
  }
}

void lights_on() {
  for (int i = 0; i < 4; i++)
  {
    dataArray[i] = Wire.read();
    Serial.print(dataArray[i]);
  }
  r_data = dataArray[0];
  b_data = dataArray[1];
  g_data = dataArray[2];
  c = dataArray[3];
}


void datas() {
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
