/**
 * @file main.cpp
 * @brief SprintGate: Sprintzeitmessung mit zwei Lichtschranken.
 *
 * Diese Datei verbindet nur die Bausteine:
 *   LightBarrier -> SprintTimer -> SprintDisplay / PulseLed / Serial
 *
 * Ablauf eines Schleifendurchlaufs:
 *   1. Zeitstempel nehmen, bevor irgendetwas Langsames passiert.
 *   2. Beide Lichtschranken abfragen.
 *   3. Auslösungen an den Zeitgeber weitergeben.
 *   4. LEDs, Display und serielle Ausgabe bedienen.
 */
#include <Arduino.h>

#include "LightBarrier.h"
#include "PulseLed.h"
#include "SprintDisplay.h"
#include "SprintTimer.h"
#include "TimeFormat.h"
#include "config.h"

namespace {

LightBarrier startGate(config::pins::kGateStart, config::kGateInputMode,
                       config::kGateActiveLevel);
LightBarrier finishGate(config::pins::kGateFinish, config::kGateInputMode,
                        config::kGateActiveLevel);

PulseLed startLed(config::pins::kLedStart, config::timing::kLedPulseUs);
PulseLed finishLed(config::pins::kLedFinish, config::timing::kLedPulseUs);

SprintTimer timer(config::timing::kRearmLockoutUs);

SprintDisplay display(config::pins::kLcdRs, config::pins::kLcdEnable,
                      config::pins::kLcdData4, config::pins::kLcdData5,
                      config::pins::kLcdData6, config::pins::kLcdData7);

/// Gibt die gemessene Zeit einmalig über die serielle Schnittstelle aus.
void printResult(uint32_t durationUs) {
  char seconds[16];
  formatSeconds(durationUs, seconds, sizeof(seconds));
  Serial.println(seconds);
}

}  // namespace

void setup() {
  startGate.begin();
  finishGate.begin();
  startLed.begin();
  finishLed.begin();

  display.begin();
  display.update(timer, micros(), true);

  Serial.begin(config::kSerialBaud);
}

void loop() {
  // Zeitstempel zuerst nehmen. Display- und LED-Zugriffe kosten Zeit und
  // dürfen die Messung nicht verfälschen.
  const uint32_t nowUs = micros();

  const bool startTriggered = startGate.pollTrigger();
  const bool finishTriggered = finishGate.pollTrigger();

  bool stateChanged = false;

  // Start vor Ziel auswerten, damit die Reihenfolge auch bei gleichzeitiger
  // Auslösung logisch bleibt.
  if (startTriggered && timer.handleStartTrigger(nowUs)) {
    startLed.trigger(nowUs);
    stateChanged = true;
  }

  if (finishTriggered && timer.handleFinishTrigger(nowUs)) {
    finishLed.trigger(nowUs);
    printResult(timer.elapsedUs(nowUs));
    stateChanged = true;
  }

  startLed.update(nowUs);
  finishLed.update(nowUs);

  // Bei Zustandswechsel sofort aktualisieren, sonst im festen Takt.
  display.update(timer, nowUs, stateChanged);
}
