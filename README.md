# My Ping Pong LED Clock

Ping pong ball diffusers on LED strips to form a digital clock/fancy display!

![](rainbow_bg.png)

I followed the build instructions from here [https://www.instructables.com/Ping-Pong-Ball-LED-Clock/](https://www.instructables.com/Ping-Pong-Ball-LED-Clock/). My own take on building my animations is described in this [blog post](https://yiweimao.github.io/blog/ping_pong_led_clock/).

Whole software fits on an Arduino (ATmega328P) Nano. Sketch compiles (without scrolling text support) to:
- Sketch uses 10236 bytes (33%) of program storage space. Maximum is 30720 bytes.
- Global variables use 1807 bytes (88%) of dynamic memory, leaving 241 bytes for local variables. Maximum is 2048 bytes.

The following foreground and background modes can be mixed and matched!

Foreground Modes:
- `T`: Single colour time mode
- `R`: Scrolling rainbow time mode
- `N`: No time
- `C`: Cycle through all digits 0--9999 quickly. 
- `is_slanted`: Option to use slanted digits or original digits (from https://www.instructables.com/Ping-Pong-Ball-LED-Clock/)

Background Animation Modes:
- `R`: Scrolling rainbow background
- `B`: No background
- `T`: Twinkle
- `F`: Fireworks
- `W`: Thunderstorm
- `H`: Firepit (works well with single colour time mode set to a light teal)

![](https://yiweimao.github.io/blog/images/2021-01-05-LED_clock_files/fireworks.gif)

Future Improvements:
- Use a hardware RTC rather than use software
- Implement scolling text (https://github.com/PlanetaryMotion/pingPongBallClock)
- Use FastLED colour palettes
- Attach light sensor and auto-adjust FastLED brightness
- Attach PIR motion sensor and turn on display when there is someone to look at it
- Attach temperature/humidity/pressure sensor and display stats
- Connect to Wifi (e.g. using an ESP32) for real time stats/remote control