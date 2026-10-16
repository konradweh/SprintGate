/**
 * @file PulseLed.h
 * @brief LED, die nach einem Auslöser für eine feste Zeit leuchtet.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * Schaltet eine LED nicht blockierend für eine feste Dauer ein.
 *
 * Die Klasse arbeitet ohne delay(). update() muss regelmäßig aufgerufen
 * werden und schaltet die LED nach Ablauf der Dauer wieder aus.
 */
class PulseLed {
 public:
  /**
   * @param pin         Arduino-Pin der LED (HIGH = an).
   * @param durationUs  Leuchtdauer in Mikrosekunden.
   */
  PulseLed(uint8_t pin, uint32_t durationUs);

  /// Konfiguriert den Pin als Ausgang und schaltet die LED aus.
  void begin();

  /// Schaltet die LED ein und startet die Leuchtdauer neu.
  void trigger(uint32_t nowUs);

  /// Schaltet die LED aus, sobald die Leuchtdauer abgelaufen ist.
  void update(uint32_t nowUs);

 private:
  const uint8_t pin_;
  const uint32_t durationUs_;
  uint32_t onSinceUs_ = 0;
  bool isOn_ = false;
};
