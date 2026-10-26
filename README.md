# Line Maze Solver — Project X 3.0

Selection-task work for the Robotronics Club's LMS / Meshmerize project.

## Repository layout
- `src/`: Python centroid/clamping functions and an ESP32 Arduino C++ line-following sketch.
- `tests/`: unit tests for Task 1.
- `BOM/`: draft bill of materials and power budget.
- `docs/`: control theory, datasheet notes, troubleshooting, telemetry, maze algorithm and references.
- `diagrams/`: wiring, power-block and example-maze diagrams where included.

## Run the Python example
From the repository root:
```bash
python3 src/task1_basic_coding.py
```

Run the tests:
```bash
python3 -m unittest discover -s tests -v
```

## ESP32 sketch
Open `src/lms_line_follower.ino` in Arduino IDE with an ESP32 board selected. The sketch defaults to `USE_SAMPLE_INPUT 1` so it prints hard-coded sensor samples for logic demonstration without driving hardware. Set it to `0` only after checking the wiring, selecting a compatible ESP32 Arduino core, and resolving the pin issue below. It prints sensor readings, estimated position, and motor commands at 115200 baud.

## Important hardware issue to resolve before physical use
Appendix A assigns GPIO11 to L298N IN4. On typical ESP32-WROOM-32 modules, GPIO6–GPIO11 are connected internally to SPI flash and should not be used as general-purpose outputs. The sketch preserves the assignment's listed mapping but deliberately does not enable hardware outputs by default. Ask the club for a corrected motor-driver pin map before connecting the L298N. Also verify whether the selected ESP32 Arduino core supports `analogWrite()` as used here, and calibrate sensor minimum/maximum values on the actual surface.

## Assumptions and limitations
- Sensor readings are calibrated, non-negative, 12-bit values; darker line produces higher values.
- Sensor weights are `[-2, -1, 0, 1, 2]`; negative position means left and positive means right.
- The sample sketch demonstrates centroid/PID logic and a safe stop if all readings are zero. It is not a claim of physical testing.
- BOM prices, shipping and current values are planning estimates and must be verified against exact product listings and datasheets.
- The power budget is provisional until exact motor model, stall current, battery capacity/C-rating and driver thermal limits are selected.
- Maze solving uses DFS for exploration and BFS for shortest-path replay on an unweighted known graph.

## Official requirements
This repository is structured to address all nine tasks in the provided selection-task PDF. Review all figures and design choices before submission; selectors may ask you to explain them.
