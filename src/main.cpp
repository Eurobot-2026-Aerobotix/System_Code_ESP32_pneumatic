#include <Arduino.h>

int front_suction_cup_pins[] = {16, 17, 12, 13};
int back_suction_cup_pins[] = {5, 21, 22, 33};
int front_pump_pin = 19;
int back_pump_pin = 18;

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
    if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() > 0) {
      int n = s.toInt();
      Serial.print("Received: ");
      Serial.println(n);

      if (n >= 0 && n <= 3) {
        Serial.println("Activating front suction cup");
        activateSuctionCup(0, n);
        delay(500);
        deactivateSuctionCup(0, n);
      } else if (n >= 4 && n <= 7) {
        int idx = n - 4;
        Serial.println("Activating back suction cup");
        activateSuctionCup(1, idx);
        delay(500);
        deactivateSuctionCup(1, idx);
      } else if (n == 20) {
        Serial.println("Activating all front suction cups");
        activateAllSuctionCups(0);
      } else if (n == 21) {
        Serial.println("Activating all back suction cups");
        activateAllSuctionCups(1);
      } else if (n == 30) {
        Serial.println("Activating front pump");
        activatePump(0);
      } else if (n == 31) {
        Serial.println("Activating back pump");
        activatePump(1);
      } else if (n == 50) {
        Serial.println("Deactivating front pump");
        deactivatePump(0);
      } else if (n == 51) {
        Serial.println("Deactivating back pump");
        deactivatePump(1);
      } else if (n == 40) {
        Serial.println("Deactivating all front suction cups");
        deactivateAllSuctionCups(0);
      } else if (n == 41) {
        Serial.println("Deactivating all back suction cups");
        deactivateAllSuctionCups(1);
      } else {
        Serial.println("Unknown command");
      }
    }
  }
  delay(10);
}
