
# ESP32 Traffic lights - Learning Project

This is a small educational project for the [Embedded Development course](https://beetroot.academy/courses/online/kurs-embedded-development)

## Circuit Diagram

```
ESP32-S3

                    +----------------------+
3.3V ---------------|                      |
                    |      ESP32-S3        |
                    |                      |
GPIO5 --------------|                      |
GPIO6 --------------|                      |
GPIO7 --------------|                      |
GPIO8 --------------|                      |
                    |                      |
GND ----------------|                      |
                    +----------------------+


BUTTON

                 +3.3V
                   │
                [BUTTON]
                   │
GPIO8 ─────────────┼───────────────
                   │
                [10kΩ]
                   │
                  GND


RED LED

GPIO5 ───[220Ω]───|>|────────────── GND
                  RED


YELLOW LED

GPIO6 ───[220Ω]───|>|────────────── GND
                YELLOW


GREEN LED

GPIO7 ───[220Ω]───|>|────────────── GND
                 GREEN

```

## Result

![Blink Result](./result.gif)

If GIF preview is not displayed in your viewer, open it directly: [result.gif](./result.gif)