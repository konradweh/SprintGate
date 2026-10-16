/**
 * @file TimeFormat.h
 * @brief Formatierung von Zeiten für Display und serielle Ausgabe.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * Schreibt eine Dauer als Sekunden mit drei Nachkommastellen, zum Beispiel
 * "12.345". Die Auflösung der Ausgabe ist eine Millisekunde, weitere
 * Mikrosekunden werden abgeschnitten.
 *
 * Die Funktion rechnet bewusst nur mit ganzen Zahlen. Gleitkomma wäre hier
 * ungenauer, und snprintf unterstützt Gleitkomma auf dem SAMD-Core nicht.
 *
 * @param durationUs  Dauer in Mikrosekunden.
 * @param buffer      Zielpuffer. Acht Zeichen plus Endnull reichen für den
 *                    gesamten Wertebereich von uint32_t.
 * @param bufferSize  Größe des Puffers in Bytes.
 */
void formatSeconds(uint32_t durationUs, char* buffer, size_t bufferSize);
