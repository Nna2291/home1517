#include <Servo.h>
#include <Wire.h>

Servo servo;
#define SERVO_PIN 10

Servo lock;
#define LOCK_PIN 11

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 9

#define B_LED 8
#define G_LED 7
#define R_LED 12

#define BUTTON_PIN 2
boolean buttonPrevState;
int timesButLength = 4;
int times_but[] = {-1, -1, -1, -1};
int timee_but, prevTime_but, delta_but;
boolean but_stat;

boolean forcePrevState;
int forceCount;
int timesForceLength = 4;
int times_force[] = {-1, -1, -1, -1};
int timee_force, prevTime_force, delta_force;
boolean force_stat;
int prevForce = 0;
int nowForce = 0;
int forces[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float max_force = 0;

const int FSR_PIN = A0;

int stage1 = 1;
int stage2 = 1;

boolean forceNow;
int r_data = 0;
int g_data = 0;
int b_data = 0;
int l_s = 0;
int d_s = 0;
byte dataArray[5];

// Функция вхождения числа в массив
int inn(int a[4], int b) {
  for (int i = 0; i < 4; i++) {
    if (a[i] == b) {
      return i;
    }
  }
  return -1;
}


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  pinMode(FSR_PIN, INPUT);

  servo.attach(SERVO_PIN);
  servo.write(180);

  lock.attach(LOCK_PIN);
  lock.write(0);

  Wire.begin(8);
  Wire.onReceive(to_slave);

  Serial.begin(9600);
}

// Функция плавного открытия замка и двери
void go(Servo servo, Servo lock, int door) {
  if (door) {
    for (int i = 10; i < 91; i += 10) {
      lock.write(i);
      delay(100);
    }
    delay(1000);
    servo.write(0); // открыть дверь
  }
  else {
    servo.write(180);
    delay(1000);
    for (int i = 80; i > -1; i -= 10) {
      lock.write(i);
      delay(100);
    }
  }
}

void loop() {
  if (but_stat) {  // Если комбинация кнопки сработала, то переходим на следующий этап
    if (stage1) {
      Serial.println("Светодиод загорается желтым");
      stage1 = 0;
      for (int i = 0; i < 4; i++) {
        times_force[i] = -1;
      }

      for (int j = 0; j < 100; j++) {
        get_force();
      }
      int summ = 0;
      for (int j = 0; j < 100; j++) {
        summ += forces[j];
      }
      float avgg = summ / 100;
      max_force = avgg + 7;
      // В части кода выше задаем порог датчика силы на семь больше, чем среднее арифметическое показаний датчиков за 100 вызовов функции
    }
    force_stat = check_pass_force();
    if (force_stat) {
      if (stage2) {
        Serial.println("Светодиод загорается зеленым, дверь открывается");
        digitalWrite(R_LED, 0);
        digitalWrite(G_LED, 1);
        digitalWrite(B_LED, 0);

        // Открываем дверь
        go(servo, lock, 1);
        
        stage2 = 0;
        delay(15000);

        Serial.println("Дверь закрывается, светодиод загорается красным");
        digitalWrite(R_LED, 1);
        digitalWrite(G_LED, 0);
        digitalWrite(B_LED, 0);

        // Закрываем дверь
        go(servo, lock, 0);

        stage2 = 1;
        stage1 = 1;

        // Ставим исходные значения
        for (int i = 0; i < 4; i++) {
          times_force[i] = -1;
          times_but[i] = -1;
        }
        but_stat = false;
        force_stat = false;
      }
    }
    else {
      digitalWrite(R_LED, 1);
      digitalWrite(G_LED, 0);
      digitalWrite(B_LED, 1);
    }
  }
  else {
    digitalWrite(R_LED, 1);
    digitalWrite(G_LED, 0);
    digitalWrite(B_LED, 0);
    stage1 = 1;
    Serial.println("Светодиод горит красным");
  }
  but_stat = check_pass_but();  // Проверяем комбинацию
}

void to_slave() {
  // Считываем данные с master
  for (int i = 0; i < 5; i++)
  {
    dataArray[i] = Wire.read();
    // Serial.print(dataArray[i]);
  }
  r_data = dataArray[0];
  b_data = dataArray[1];
  g_data = dataArray[2];
  l_s = dataArray[3];
  d_s = dataArray[4];

  if (d_s) {
    go(servo, lock, 1);
  }
  else {
    go(servo, lock, 0);
  }

  if (l_s == 1) {
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

// Проверка на нажатие на датчик силы
boolean check_force()
{
  get_force();
  if (forcePrevState && !forceNow)
  {
    forcePrevState = forceNow;
    return true;
  }
  forcePrevState = forceNow;
  return false;
}

// Считывание данных с датчика силы и алгоритм высчитывания среднего арифметического за последние 100 вызовов функции и его сравнения с порогом
void get_force() {
  nowForce = analogRead(FSR_PIN);
  int sum = 0;
  for (int i = 0; i < 100; i++) {
    if (i != 99) {
      forces[i] = forces[i + 1];
    }
    else if (i == 99) {
      forces[i] = nowForce;
    }
  }
  for (int i = 0; i < 100; i++) {
    sum += forces[i];
  }
  float avg = sum / 100;
  if (avg > max_force) {
    Serial.println(avg);
    forceNow = true;
  }
  else {
    forceNow = false;
  }
  prevForce = forceNow;
}

// Проверка на нажатие на кнопку
boolean check_button() {
  boolean buttonState = digitalRead(BUTTON_PIN);
  if ( (buttonPrevState == HIGH) && (buttonState == LOW) ) {
    buttonPrevState = buttonState;
    return true;
  }
  buttonPrevState = buttonState;
  return false;
}

// Определение интервалов времени для кнопки
boolean check_pass_but() {
  if (check_button()) {
    timee_but = millis();
    delta_but = timee_but - prevTime_but;
    prevTime_but = timee_but;
    for (int i = 0; i < timesButLength; i++) {
      if (times_but[timesButLength - 1] == -1) {
        if (times_but[i] == -1) {
          times_but[i] = delta_but;
          break;
        }
      }
      else {
        if (i != timesButLength - 1) {
          times_but[i] = times_but[i + 1];
        }
        else {
          times_but[i] = delta_but;
        }
      }
    }
  }
  //  for (int i = 0; i < 4; i++) {
  //    Serial.println(times_but[i]);
  //  }
  //  Serial.println(". . . . . . . . . . . .");
  if ((300 <= times_but[0] && times_but[0] < 1000) && (50 < times_but[1] < 300) && (300 <= times_but[2] && times_but[2] < 1000) && (300 <= times_but[3] && times_but[3] < 1000)) {
    return true;
  }
  return false;
}

// Определение интервалов времени для датчика силы
boolean check_pass_force() {
  if (check_force()) {
    timee_force = millis();
    delta_force = timee_force - prevTime_force;
    prevTime_force = timee_force;
    for (int i = 0; i < timesForceLength; i++) {
      if (times_force[timesForceLength - 1] == -1) {
        int a = inn(times_force, -1);
        if (a != -1) {
          if (delta_force >= 100) {
            times_force[a] = delta_force;
          }
          break;
        }
      }
      else {
        if (forceCount == 1) {
          forceCount = 0;
          stage2 = 1;
          stage1 = 1;
          for (int i = 0; i < 4; i++) {
            times_force[i] = -1;
            times_but[i] = -1;
          }
          but_stat = false;
          force_stat = false;
          return false;
        }
        if (i == timesForceLength - 1) {
          if (delta_force >= 100) {
            times_force[i] = delta_force;
          }
          else {
            times_force[i] = -1;
          }
          forceCount += 1;
        }
        else {
          times_force[i] = times_force[i + 1];
        }
      }
    }
  }
//  for (int i = 0; i < 4; i++) {
//    Serial.println(times_force[i]);
//  }
//  Serial.println(". . . . . . . . . . . . . . .");
  if ((times_force[0] < 1000) && (1000 <= times_force[1] && times_force[1] < 2000) && (times_force[2] < 1000) && (1000 <= times_force[3] && times_force[3] < 2000)) {
    return true;
  }
  return false;
}