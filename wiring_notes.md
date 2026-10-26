# Task 6 — wiring notes (verify before hardware use)

- Five analog sensor outputs connect to GPIO 34, 35, 32, 33 and 25 as listed in Appendix A. Sensor supply must match the exact array's rating and outputs must remain within ESP32 ADC limits (0–3.3 V).
- L298N motor supply is the 11.1 V battery rail. ESP32 must receive a regulated supply suitable for its Vin/5 V input; never feed 11.1 V directly to the 3V3 pin.
- Join battery negative, ESP32 GND and L298N GND to establish a common logic reference.
- Connect ENA/IN1/IN2 and ENB/IN3/IN4 according to Appendix A only after resolving the GPIO11 conflict in README. GPIO6–GPIO11 are normally reserved for flash on ESP32-WROOM-32.
- Fit a correctly rated fuse/switch and use a suitable LiPo balance charger. Confirm motor stall current, L298N thermal limits, battery discharge rating and connector polarity before powering the system.
