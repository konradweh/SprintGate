/**
 * @file LightBarrier.h
 * @brief Einzelne Lichtschranke mit Flankenerkennung.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * Liest eine Lichtschranke und meldet jede neue Unterbrechung genau einmal.
 *
 * Die Klasse wertet die Flanke aus (Strahl frei -> Strahl unterbrochen) und
 * nicht den Pegel. Eine dauerhaft blockierte Schranke löst deshalb nur einmal
 * aus. Beim Einschalten wird der aktuelle Zustand als Ausgangslage übernommen,
 * ein bereits blockierter Strahl erzeugt also keinen Fehlauslöser.
 */
class LightBarrier {
 public:
  /**
   * @param pin          Arduino-Pin des Sensors.
   * @param inputMode    INPUT oder INPUT_PULLUP.
   * @param activeLevel  Pegel bei unterbrochenem Strahl (LOW oder HIGH).
   */
  LightBarrier(uint8_t pin, uint8_t inputMode, uint8_t activeLevel);

  /// Konfiguriert den Pin und liest den Ausgangszustand. Einmal in setup() aufrufen.
  void begin();

  /**
   * Fragt den Sensor ab. Möglichst oft aufrufen, am besten einmal pro loop().
   * @return true genau dann, wenn der Strahl seit dem letzten Aufruf neu
   *         unterbrochen wurde.
   */
  bool pollTrigger();

 private:
  /// Liest den Pin und wandelt den Pegel in "Strahl unterbrochen".
  bool isInterrupted() const;

  const uint8_t pin_;
  const uint8_t inputMode_;
  const uint8_t activeLevel_;
  bool wasInterrupted_ = false;  ///< Zustand beim letzten Aufruf.
};
