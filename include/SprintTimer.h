/**
 * @file SprintTimer.h
 * @brief Zustandsautomat für einen Sprintlauf (reine Logik, ohne Hardware).
 */
#pragma once

#include <stdint.h>

/**
 * Verwaltet den Ablauf eines Laufs: bereit, läuft, beendet.
 *
 * Die Klasse greift nicht auf Hardware zu. Alle Zeitpunkte kommen als
 * Parameter von außen (Mikrosekunden, wie von micros()). Dadurch lässt sie
 * sich auf dem PC testen und später unverändert für Funkmodule verwenden.
 *
 * Zustandsübergänge:
 *
 *     kIdle --Start--> kRunning --Ziel--> kFinished
 *       ^                                    |
 *       +---- (Sperrzeit abgelaufen, Start) -+--> kRunning
 *
 * Ein Start während eines laufenden Laufs wird ignoriert, ebenso ein Ziel
 * ohne vorherigen Start. Nach dem Ziel nimmt die Klasse erst nach der
 * Sperrzeit wieder einen Start an.
 */
class SprintTimer {
 public:
  enum class State : uint8_t {
    kIdle,      ///< Bereit, es wurde noch kein Lauf gestartet.
    kRunning,   ///< Lauf aktiv, die Zeit läuft.
    kFinished,  ///< Lauf beendet, das Ergebnis steht fest.
  };

  /// @param rearmLockoutUs  Sperrzeit nach dem Ziel, bevor ein Start gilt.
  explicit SprintTimer(uint32_t rearmLockoutUs);

  /**
   * Meldet eine Auslösung der Startschranke.
   * @return true, wenn dadurch ein neuer Lauf gestartet wurde.
   */
  bool handleStartTrigger(uint32_t nowUs);

  /**
   * Meldet eine Auslösung der Zielschranke.
   * @return true, wenn dadurch ein laufender Lauf beendet wurde.
   */
  bool handleFinishTrigger(uint32_t nowUs);

  State state() const { return state_; }

  /**
   * Zeit des aktuellen oder letzten Laufs in Mikrosekunden.
   * - kIdle:     0
   * - kRunning:  Zeit seit dem Start
   * - kFinished: endgültige Laufzeit
   */
  uint32_t elapsedUs(uint32_t nowUs) const;

 private:
  const uint32_t rearmLockoutUs_;
  State state_ = State::kIdle;
  uint32_t startUs_ = 0;
  uint32_t finishUs_ = 0;
};
