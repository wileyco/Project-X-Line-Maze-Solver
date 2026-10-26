# Technical notes: Line Maze Solver

## Task 2 — Datasheet literacy: L298N

The selection task's Appendix A specifies an L298N dual H-bridge driver. The design must check the exact module/IC datasheet before assembly. The three key checks are: (1) motor supply voltage range, because the robot's stated motor rail is 11.1 V; (2) continuous and peak output current per channel, which must cover the selected motor's running current and start/stall current without exceeding thermal limits; and (3) logic-input thresholds, because the controller is an ESP32 with 3.3 V GPIO. The chosen breakout's input thresholds and onboard regulator arrangement must be verified, not assumed from a generic module photo.

A damaging condition is exceeding the driver's absolute-maximum current or junction-temperature rating. The design should select motors whose stall current is within the driver's permitted pulse/thermal limits, avoid prolonged stall, and provide suitable cooling. L298N bipolar output stages have a comparatively large voltage drop and dissipate heat. A TB6612FNG MOSFET driver is a possible lower-loss alternative, but the assignment's Appendix A explicitly specifies L298N; therefore the wiring and implementation here follow that specification.

## Task 3 — PID control

The five calibrated sensor readings are combined into a weighted centroid using weights [-2, -1, 0, 1, 2]. The centre position is zero; negative values mean the line is left of centre and positive values mean it is right. This position is the steering error. Proportional control responds to the current error. Integral control accumulates error and can correct persistent mechanical bias, but can wind up during turns or when the line is lost. Derivative control responds to the rate of change and can reduce oscillation, although noisy readings can make it sensitive.

For this robot, PD is the starting choice (Ki = 0). Integral action is not initially needed for line following and can cause overshoot; it can be added only if testing demonstrates persistent bias. The correction is applied differentially: left = base + correction and right = base - correction, with each output constrained to 0–255 for forward-only operation. If oscillation appears, reduce Kp or increase Kd gradually; tune on a safe, repeatable track. If all sensor readings are zero, the centroid is undefined. The implementation stops both motors safely instead of continuing with an arbitrary error.

## Task 6 — Troubleshooting: ESP32 resets when motors start

A likely cause is supply sag or electrical noise when motor startup current rises toward stall current. First check the battery voltage at the controller supply during motor start and inspect whether motor and logic supplies share a stable ground. Confirm the ESP32 is powered from a suitable regulated 5 V/Vin or 3.3 V source, never directly from the 11.1 V battery. Confirm the L298N's regulator jumper and output are used only as permitted by the exact module documentation. Then add suitable bulk capacitance across the motor supply, 0.1 µF suppression capacitors at brushed motor terminals, separate motor and logic power routing, and a common ground. The fix is confirmed by repeating starts while observing the controller supply and checking that resets no longer occur. Do not connect hardware until the Appendix A pin conflict noted in README is resolved.

## Task 7 — Wireless sensor diagnostics

Use BLE for short-range live telemetry to a phone or laptop because the ESP32 supports it without an external router and it is suitable for sending small diagnostic frames with modest overhead. A practical flow is: sensor sampling → calibration → per-channel anomaly checks → timestamped frame → BLE characteristic notification → phone terminal/logger → plot or CSV log. A frame can include five readings, estimated position, error, motor commands and a fault bitmask.

A candidate fault is a single channel abruptly dropping to zero or saturating near 4095 while neighbouring channels remain stable. A legitimate line shift usually produces a gradual change across neighbouring sensors. Use a short rolling window and thresholds based on calibration; require repeated anomalies before flagging a fault so a real junction or surface change is not immediately misclassified. Continue line control during diagnosis. If a sensor fault makes position unreliable, use the controller's safe lost-line response and report the fault; wireless telemetry itself must not block the control loop.

## Task 8 — Maze-solving algorithms

A left- or right-hand rule is simple and memory-light, but it does not guarantee reaching the exit in mazes with loops or disconnected wall structures. Depth-first search (DFS) is preferable for an unknown maze when the robot can detect junctions, record its choices, and mark visited edges. Represent junctions as nodes and traversable corridors as edges. At a node, record available exits, choose an unvisited edge, and backtrack when all exits have been explored. This eventually explores every reachable edge in a finite graph, assuming reliable junction detection and successful backtracking.

Exploration and replay are different phases. During exploration the map is incomplete, so DFS records a route and dead ends. Once the graph is known, run BFS on an unweighted graph to find a shortest path in number of edges, then replay that route. If turn time or corridor length matters, use weighted shortest-path search instead. Example diagram and a sample graph are included in `diagrams/maze_example.png` if available; otherwise use the textual graph below: A connects to B and C; B connects to dead end D; C connects to E (exit). DFS may explore A→B→D, backtrack to A, then explore A→C→E. Replay can use A→C→E.
