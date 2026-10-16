#include <Arduino.h>
#include <LiquidCrystal.h>

#include "config.h"

LiquidCrystal lcd(pins::kLcdRs, pins::kLcdEn, pins::kLcdD4, pins::kLcdD5, pins::kLcdD6, pins::kLcdD7);

// State of the current run
unsigned long startMs = 0;
unsigned long endMs = 0;       // 0 = run is still active
unsigned long durationMs = 0;
bool resultPrinted = false;

void showTime(unsigned long ms) {
  lcd.setCursor(0, 0);
  lcd.print("Dauer:");
  lcd.setCursor(0, 1);
  lcd.print(ms / 1000.0, 3);
  lcd.print(" s");
}

void setup() {
  pinMode(pins::kGateStart, INPUT);
  pinMode(pins::kGateEnd, INPUT);
  pinMode(pins::kLedStart, OUTPUT);
  pinMode(pins::kLedEnd, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(kSerialBaud);
}

void loop() {
  const unsigned long now = millis();

  // Start gate: new run, but only after the re-arm delay
  if (digitalRead(pins::kGateStart) == LOW && (now - endMs) > kRearmDelayMs) {
    startMs = now;
    durationMs = 0;
    endMs = 0;
    resultPrinted = false;
    lcd.setCursor(5, 1);
    lcd.print("   ");
    digitalWrite(pins::kLedStart, HIGH);
  }
  if ((now - startMs) > kLedPulseMs) digitalWrite(pins::kLedStart, LOW);

  // Finish gate: stop the run (only once)
  if (digitalRead(pins::kGateEnd) == LOW && endMs == 0) {
    endMs = now;
    durationMs = endMs - startMs;
    digitalWrite(pins::kLedEnd, HIGH);
  }
  if ((now - endMs) > kLedPulseMs) digitalWrite(pins::kLedEnd, LOW);

  // Display and serial output
  if (endMs == 0) {
    showTime(now - startMs);
  } else {
    showTime(durationMs);
    if (!resultPrinted) {
      Serial.println(durationMs / 1000.0, 3);
      resultPrinted = true;
    }
  }
}
