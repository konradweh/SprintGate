# SprintGate

Wireless, expandable timing gates for sprint measurement. Originally built for a field hockey club as a
wired two-gate setup, now being turned into a modular system with radio links between small controller modules.

## Status

Step 1: The original Arduino IDE sketch (two wired light barriers, 16x2 LCD) is ported to a CLion/PlatformIO
project for the Arduino Nano 33 IoT and rewritten as clean, hardware-independent C++.

## Structure

| File | Purpose |
|------|---------|
| `include/config.h` | Pins, timings, display geometry |
| `LightBarrier` | Reads one gate, reports each new interruption exactly once (edge detection) |
| `SprintTimer` | State machine (idle, running, finished), pure logic without hardware access |
| `PulseLed` | Non-blocking LED pulse |
| `SprintDisplay` | 16x2 LCD output, throttled and only writes changed lines |
| `TimeFormat` | Integer-only formatting of durations |
| `src/main.cpp` | Wires the parts together |

## Roadmap

- [ ] Modules with their own small controller, wireless communication instead of cables
- [ ] Expandable number of gates (split times)
- [ ] Display for reading the time
- [ ] Status LEDs and several buttons on the PCB
- [ ] Custom PCB

## Build

Requires CLion with the PlatformIO plugin (or `pio` on the command line).

```
pio run                 # build
pio run -t upload       # flash
pio device monitor      # serial monitor
```

Pins and timings are in `include/config.h`.
