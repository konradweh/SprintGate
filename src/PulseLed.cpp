#include "PulseLed.h"

PulseLed::PulseLed(uint8_t pin, uint32_t durationUs)
    : pin_(pin), durationUs_(durationUs) {}

void PulseLed::begin() {
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
  isOn_ = false;
}

void PulseLed::trigger(uint32_t nowUs) {
  digitalWrite(pin_, HIGH);
  onSinceUs_ = nowUs;
  isOn_ = true;
}

void PulseLed::update(uint32_t nowUs) {
  // Die Subtraktion mit vorzeichenlosen Zahlen bleibt auch beim Überlauf von
  // micros() (nach ca. 71 Minuten) korrekt.
  if (isOn_ && (nowUs - onSinceUs_) >= durationUs_) {
    digitalWrite(pin_, LOW);
    isOn_ = false;
  }
}
