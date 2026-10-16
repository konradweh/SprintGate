#include "LightBarrier.h"

LightBarrier::LightBarrier(uint8_t pin, uint8_t inputMode, uint8_t activeLevel)
    : pin_(pin), inputMode_(inputMode), activeLevel_(activeLevel) {}

void LightBarrier::begin() {
  pinMode(pin_, inputMode_);
  // Ausgangslage merken, damit ein beim Start schon unterbrochener Strahl
  // nicht sofort als Auslösung zählt.
  wasInterrupted_ = isInterrupted();
}

bool LightBarrier::pollTrigger() {
  const bool interrupted = isInterrupted();
  const bool newlyInterrupted = interrupted && !wasInterrupted_;
  wasInterrupted_ = interrupted;
  return newlyInterrupted;
}

bool LightBarrier::isInterrupted() const {
  return digitalRead(pin_) == activeLevel_;
}
