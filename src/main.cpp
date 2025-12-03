#include <Arduino.h>
int front_suction_cup_pins[] = {22, 33, 21, 5};
int back_suction_cup_pins[] = {13, 16, 17, 12};
int front_pump_pin = 18;
int back_pump_pin = 19;

void setup() {
  Serial.begin(115200);
  for (int pin : front_suction_cup_pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (int pin : back_suction_cup_pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(front_pump_pin, OUTPUT);
  digitalWrite(front_pump_pin, LOW);
  pinMode(back_pump_pin, OUTPUT);
  digitalWrite(back_pump_pin, LOW);
}

void activateSuctionCup(int front_0_back_1, int cup_index) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_suction_cup_pins[cup_index], HIGH);
  } else {
    digitalWrite(back_suction_cup_pins[cup_index], HIGH);
  }
}
void activateAllSuctionCups(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    for (int pin : front_suction_cup_pins) {
      digitalWrite(pin, HIGH);
    }
  } else {
    for (int pin : back_suction_cup_pins) {
      digitalWrite(pin, HIGH);
    }
  }
}
void deactivateSuctionCup(int front_0_back_1, int cup_index) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_suction_cup_pins[cup_index], LOW);
  } else {
    digitalWrite(back_suction_cup_pins[cup_index], LOW);
  }
}
void deactivateAllSuctionCups(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    for (int pin : front_suction_cup_pins) {
      digitalWrite(pin, LOW);
    }
  } else {
    for (int pin : back_suction_cup_pins) {
      digitalWrite(pin, LOW);
    }
  }
}
void activatePump(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_pump_pin, HIGH);
  } else {
    digitalWrite(back_pump_pin, HIGH);
  }
}
void deactivatePump(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_pump_pin, LOW);
  } else {
    digitalWrite(back_pump_pin, LOW);
  }
}
void loop() {
}
