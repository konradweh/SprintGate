#include "SprintDisplay.h"

#include <stdio.h>
#include <string.h>

#include "TimeFormat.h"

SprintDisplay::SprintDisplay(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5,
                             uint8_t d6, uint8_t d7)
    : lcd_(rs, enable, d4, d5, d6, d7) {}

void SprintDisplay::begin() {
  lcd_.begin(config::kLcdColumns, config::kLcdRows);
}

void SprintDisplay::update(const SprintTimer& timer, uint32_t nowUs,
                           bool force) {
  if (!force && (nowUs - lastRefreshUs_) < config::timing::kDisplayRefreshUs) {
    return;
  }
  lastRefreshUs_ = nowUs;

  // Zeit als Text, zum Beispiel "12.345 s".
  char seconds[16];
  formatSeconds(timer.elapsedUs(nowUs), seconds, sizeof(seconds));
  char timeText[kLineBufferSize];
  snprintf(timeText, sizeof(timeText), "%s s", seconds);

  drawLine(0, titleFor(timer.state()));
  drawLine(1, timeText);
}

void SprintDisplay::drawLine(uint8_t row, const char* text) {
  // Der Text wird mit Leerzeichen auf die volle Zeilenbreite aufgefüllt.
  // Dadurch überschreibt eine kürzere Zeit die Reste einer längeren, ohne
  // dass das Display gelöscht werden muss (kein Flackern).
  char padded[kLineBufferSize];
  snprintf(padded, sizeof(padded), "%-*s", static_cast<int>(config::kLcdColumns),
           text);

  if (strcmp(padded, lastLines_[row]) == 0) {
    return;  // Unverändert, kein LCD-Zugriff nötig.
  }

  lcd_.setCursor(0, row);
  lcd_.print(padded);
  memcpy(lastLines_[row], padded, kLineBufferSize);
}

const char* SprintDisplay::titleFor(SprintTimer::State state) {
  // Nur ASCII, der HD44780 hat keine Umlaute im Standardzeichensatz.
  switch (state) {
    case SprintTimer::State::kIdle:
      return "Bereit";
    case SprintTimer::State::kRunning:
      return "Laeuft";
    case SprintTimer::State::kFinished:
      return "Dauer:";
  }
  return "";
}
