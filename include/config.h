/**
 * @file config.h
 * @brief Zentrale Konfiguration von SprintGate (Pins, Zeiten, Display).
 *
 * Alle "magischen Zahlen" des Projekts stehen hier. Wer die Verdrahtung oder
 * das Verhalten anpassen will, muss nur diese Datei ändern.
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace config {

/// Pinbelegung für den Arduino Nano 33 IoT.
namespace pins {

// Lichtschranken. Ein unterbrochener Strahl liefert LOW (siehe kGateActiveLevel).
constexpr uint8_t kGateStart = A0;
constexpr uint8_t kGateFinish = A1;

// Rückmeldungs-LEDs (HIGH = an).
constexpr uint8_t kLedStart = A3;
constexpr uint8_t kLedFinish = A5;

// LCD (HD44780, 16x2) im 4-Bit-Modus.
constexpr uint8_t kLcdRs = 12;
constexpr uint8_t kLcdEnable = 11;
constexpr uint8_t kLcdData4 = 5;
constexpr uint8_t kLcdData5 = 4;
constexpr uint8_t kLcdData6 = 3;
constexpr uint8_t kLcdData7 = 2;

}  // namespace pins

/// Betriebsart der Lichtschranken-Eingänge.
///
/// INPUT entspricht dem ursprünglichen Aufbau (die Sensorplatine treibt den
/// Pegel aktiv). Bei offenen Kontakten oder Tastern wäre INPUT_PULLUP die
/// richtige Wahl, weil der Pin sonst floatet.
constexpr uint8_t kGateInputMode = INPUT;

/// Pegel, den ein Eingang bei unterbrochenem Lichtstrahl hat.
constexpr uint8_t kGateActiveLevel = LOW;

/// Alle Zeitangaben in Mikrosekunden, passend zu micros().
namespace timing {

/// Sperrzeit nach dem Ziel, in der kein neuer Start angenommen wird.
/// So löst ein Sportler, der noch durchs Ziel läuft, keinen neuen Lauf aus.
constexpr uint32_t kRearmLockoutUs = 3'000'000;

/// Leuchtdauer der Rückmeldungs-LEDs.
constexpr uint32_t kLedPulseUs = 500'000;

/// Mindestabstand zwischen zwei Display-Aktualisierungen.
/// Jeder LCD-Zugriff blockiert die Hauptschleife kurz. Je seltener er
/// passiert, desto weniger Jitter entsteht bei der Lichtschranken-Abfrage.
constexpr uint32_t kDisplayRefreshUs = 50'000;

}  // namespace timing

/// LCD-Geometrie.
constexpr uint8_t kLcdColumns = 16;
constexpr uint8_t kLcdRows = 2;

/// Baudrate der seriellen Ausgabe (bei nativem USB ohne praktische Wirkung).
constexpr unsigned long kSerialBaud = 115200;

}  // namespace config
