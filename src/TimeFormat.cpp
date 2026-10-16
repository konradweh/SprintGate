#include "TimeFormat.h"

#include <stdio.h>

void formatSeconds(uint32_t durationUs, char* buffer, size_t bufferSize) {
  const uint32_t totalMs = durationUs / 1000U;
  const unsigned long seconds = totalMs / 1000U;
  const unsigned long millis = totalMs % 1000U;
  // %03lu füllt die Millisekunden mit führenden Nullen auf (5 ms -> ".005").
  snprintf(buffer, bufferSize, "%lu.%03lu", seconds, millis);
}
