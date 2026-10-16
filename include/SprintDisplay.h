/**
 * @file SprintDisplay.h
 * @brief Anzeige von Status und Zeit auf dem 16x2-LCD.
 */
#pragma once

#include <LiquidCrystal.h>
#include <stddef.h>
#include <stdint.h>

#include "SprintTimer.h"
#include "config.h"

/**
 * Zeigt den Zustand des SprintTimer auf dem LCD.
 *
 * Zeile 1: Status ("Bereit", "Laeuft", "Dauer:")
 * Zeile 2: Zeit in Sekunden, zum Beispiel "12.345 s"
 *
 * Zwei Maßnahmen halten die Hauptschleife schnell, damit die Lichtschranken
 * zeitnah abgefragt werden:
 *  - Das Display wird höchstens alle config::timing::kDisplayRefreshUs
 *    aktualisiert.
 *  - Es werden nur Zeilen geschrieben, deren Text sich geändert hat.
 */
class SprintDisplay {
 public:
  SprintDisplay(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6,
                uint8_t d7);

  /// Initialisiert das LCD. Einmal in setup() aufrufen.
  void begin();

  /**
   * Aktualisiert die Anzeige, wenn sie fällig ist.
   *
   * @param timer   Der darzustellende Zeitgeber.
   * @param nowUs   Aktueller Zeitpunkt (micros()).
   * @param force   true erzwingt eine sofortige Aktualisierung, etwa direkt
   *                nach einem Start oder Ziel.
   */
  void update(const SprintTimer& timer, uint32_t nowUs, bool force);

 private:
  /// Puffergröße für eine Zeile: Spalten plus Endnull.
  static constexpr size_t kLineBufferSize = config::kLcdColumns + 1;

  /// Schreibt eine Zeile, aber nur wenn sie sich vom letzten Stand unterscheidet.
  void drawLine(uint8_t row, const char* text);

  /// Liefert den Statustext der ersten Zeile.
  static const char* titleFor(SprintTimer::State state);

  LiquidCrystal lcd_;
  uint32_t lastRefreshUs_ = 0;
  /// Zuletzt gezeigter Text je Zeile. Anfangs leer, damit der erste Aufruf
  /// jede Zeile schreibt.
  char lastLines_[config::kLcdRows][kLineBufferSize] = {};
};
