#include "SprintTimer.h"

SprintTimer::SprintTimer(uint32_t rearmLockoutUs)
    : rearmLockoutUs_(rearmLockoutUs) {}

bool SprintTimer::handleStartTrigger(uint32_t nowUs) {
  switch (state_) {
    case State::kRunning:
      // Ein Lauf ist aktiv. Eine zweite Unterbrechung der Startschranke
      // (zum Beispiel durch einen Zweiten) darf die Zeit nicht zurücksetzen.
      return false;

    case State::kFinished:
      // Nach dem Ziel gilt eine Sperrzeit. Die Differenz ist auch beim
      // Überlauf von micros() korrekt.
      if ((nowUs - finishUs_) < rearmLockoutUs_) {
        return false;
      }
      break;

    case State::kIdle:
      break;
  }

  startUs_ = nowUs;
  state_ = State::kRunning;
  return true;
}

bool SprintTimer::handleFinishTrigger(uint32_t nowUs) {
  // Ein Ziel ohne Start ist bedeutungslos. Ohne diese Prüfung entstünde eine
  // Fantasiezeit aus einem veralteten Startwert.
  if (state_ != State::kRunning) {
    return false;
  }

  finishUs_ = nowUs;
  state_ = State::kFinished;
  return true;
}

uint32_t SprintTimer::elapsedUs(uint32_t nowUs) const {
  switch (state_) {
    case State::kRunning:
      return nowUs - startUs_;
    case State::kFinished:
      return finishUs_ - startUs_;
    case State::kIdle:
      break;
  }
  return 0;
}
