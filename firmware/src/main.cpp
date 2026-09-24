#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality

// put function declarations here:
int myFunction(int, int);

// pinouts
const int THUMB_PIN = A0;
const int INDEX_PIN = A1;
const int MIDDLE_PIN = A2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(THUMB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MIDDLE_PIN, INPUT);

  Serial.println("time_ms,thumb,index,middle");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long time = millis();

  int thumb = analogRead(THUMB_PIN);
  int index = analogRead(INDEX_PIN);
  int middle = analogRead(MIDDLE_PIN);

  Serial.print(time);
  Serial.print(",");
  Serial.print(thumb);
  Serial.print(",");
  Serial.print(index);
  Serial.print(",");
  Serial.println(middle);

  delay(50);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}