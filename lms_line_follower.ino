/*
 Task 9: ESP32 line follower demonstration.
 Uses Appendix A sensor pins and L298N pin assignments.
 IMPORTANT: Appendix A assigns GPIO11 to IN4. GPIO6-11 are normally
 connected to SPI flash on ESP32-WROOM-32 modules; verify the exact board
 and obtain a corrected pin map from the club before connecting hardware.
 This sketch is for sample-input validation by default; set USE_SAMPLE_INPUT
 to 0 only after checking the wiring and pin map.
*/
#define USE_SAMPLE_INPUT 1
const int sensorPins[5] = {34, 35, 32, 33, 25};
const int weights[5] = {-2, -1, 0, 1, 2};

// Appendix A L298N mapping (IN4=GPIO11 requires verification).
const int ENA = 14, IN1 = 27, IN2 = 26;
const int ENB = 13, IN3 = 12, IN4 = 11;

int sensorMin[5] = {0, 0, 0, 0, 0};
int sensorMax[5] = {4095, 4095, 4095, 4095, 4095};
float Kp = 32.0, Ki = 0.0, Kd = 18.0;
float integral = 0.0, previousError = 0.0;
int lastLineDirection = 1;
const int baseSpeed = 125;
const int maxPWM = 255;
const float dtSeconds = 0.01;

int readCalibrated(int i) {
#if USE_SAMPLE_INPUT
  const int sampleSets[4][5] = {
    {120, 850, 3000, 900, 150},
    {100, 600, 2400, 1500, 300},
    {150, 700, 2800, 800, 120},
    {0, 0, 0, 0, 0}
  };
  static int sampleIndex = 0;
  int v = sampleSets[sampleIndex][i];
  if (i == 4) sampleIndex = (sampleIndex + 1) % 4;
  return v;
#else
  int raw = analogRead(sensorPins[i]);
  int span = sensorMax[i] - sensorMin[i];
  if (span <= 0) return 0;
  return constrain((raw - sensorMin[i]) * 4095 / span, 0, 4095);
#endif
}

bool estimatePosition(const int readings[5], float &position) {
  long total = 0;
  long weighted = 0;
  for (int i = 0; i < 5; ++i) {
    total += readings[i];
    weighted += (long)weights[i] * readings[i];
  }
  if (total == 0) return false;
  position = (float)weighted / (float)total;
  return true;
}

int clampPWM(float value) {
  return constrain((int)lroundf(value), 0, maxPWM);
}

void setMotor(int pwmPin, int inA, int inB, int command) {
  // This example assumes forward direction for non-negative commands.
  // Reverse commands are supported by changing the H-bridge inputs.
  int magnitude = constrain(abs(command), 0, maxPWM);
  if (command >= 0) { digitalWrite(inA, HIGH); digitalWrite(inB, LOW); }
  else { digitalWrite(inA, LOW); digitalWrite(inB, HIGH); }
  analogWrite(pwmPin, magnitude);
}

void setup() {
  Serial.begin(115200);
#if !USE_SAMPLE_INPUT
  analogReadResolution(12);
  for (int i = 0; i < 5; ++i) pinMode(sensorPins[i], INPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
#endif
}

void loop() {
  int s[5];
  for (int i = 0; i < 5; ++i) s[i] = readCalibrated(i);

  float position = 0.0;
  bool lineFound = estimatePosition(s, position);
  int leftCommand = 0, rightCommand = 0;

  if (lineFound) {
    float error = position; // centred line is 0; right is positive
    integral += error * dtSeconds;
    integral = constrain(integral, -10.0f, 10.0f); // anti-windup
    float derivative = (error - previousError) / dtSeconds;
    float correction = Kp * error + Ki * integral + Kd * derivative;
    // Positive rightward error: slow right wheel and speed left wheel.
    leftCommand = clampPWM(baseSpeed + correction);
    rightCommand = clampPWM(baseSpeed - correction);
    if (error < 0) lastLineDirection = -1;
    if (error > 0) lastLineDirection = 1;
    previousError = error;
  } else {
    // Safe simple recovery: stop rather than drive blindly when line is lost.
    leftCommand = 0;
    rightCommand = 0;
    integral = 0;
  }

  Serial.printf("%d,%d,%d,%d,%d,%.3f,%d,%d\n",
                s[0], s[1], s[2], s[3], s[4],
                lineFound ? position : 999.0f, leftCommand, rightCommand);
#if !USE_SAMPLE_INPUT
  setMotor(ENA, IN1, IN2, leftCommand);
  setMotor(ENB, IN3, IN4, rightCommand);
#endif
  delay(10);
}
