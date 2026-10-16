#pragma once

// Pin assignment (Arduino Nano 33 IoT)
namespace pins {
constexpr uint8_t kGateStart = A0;  // light barrier at the start, LOW = beam interrupted
constexpr uint8_t kGateEnd   = A1;  // light barrier at the finish, LOW = beam interrupted
constexpr uint8_t kLedStart  = A3;  // feedback LED: start triggered
constexpr uint8_t kLedEnd    = A5;  // feedback LED: finish triggered

// 16x2 character LCD (4 bit mode)
constexpr uint8_t kLcdRs = 12;
constexpr uint8_t kLcdEn = 11;
constexpr uint8_t kLcdD4 = 5;
constexpr uint8_t kLcdD5 = 4;
constexpr uint8_t kLcdD6 = 3;
constexpr uint8_t kLcdD7 = 2;
}  // namespace pins

// Timing
constexpr unsigned long kRearmDelayMs = 3000;  // minimum time after a finish before a new start is accepted
constexpr unsigned long kLedPulseMs   = 500;   // how long the feedback LEDs stay on

constexpr unsigned long kSerialBaud = 115200;
