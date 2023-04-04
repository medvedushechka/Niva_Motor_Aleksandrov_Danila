#include <EEPROM.h>

const int ledPin = 13; // пин светодиода
const int analogInPin = A0; // пин АЦП
int sensorValue = 0; // значение с АЦП
unsigned long previousMillis = 0; // предыдущее время в миллисекундах
const long interval = 1000; // интервал между морганиями светодиода (1 секунда)
const int eepromSize = 10; // количество значений для записи во флэш-память
int eepromIndex = 0; // индекс текущего значения во флэш-памяти

void setup() {
  pinMode(ledPin, OUTPUT); // устанавливаем пин светодиода на вывод
  Serial.begin(9600); // инициализируем UART порт
}

void loop() {
  // получаем значение с АЦП
  sensorValue = analogRead(analogInPin);

  // мигаем светодиодом со скоростью, зависящей от значения с АЦП
  digitalWrite(ledPin, HIGH);
  delay(map(sensorValue, 0, 1023, 500, 50));
  digitalWrite(ledPin, LOW);
  delay(map(sensorValue, 0, 1023, 500, 50));

  // отправляем значение на компьютер каждую секунду
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(sensorValue);
  }

  // записываем значение во флэш-память каждые 5 секунд
  if (currentMillis % 5000 == 0) {
    EEPROM.write(eepromIndex, sensorValue);
    eepromIndex = (eepromIndex + 1) % eepromSize;
  }

  // очищаем флэш-память после записи 10 значений
  if (eepromIndex == 0) {
    for (int i = 0; i < eepromSize; i++) {
      EEPROM.write(i, 255); //заменяем заполненные ячейки на значение 255, так как по умолчанию незатронутые ячейки хранят в себе эти значения
    }
  }
}
