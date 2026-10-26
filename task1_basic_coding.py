"""Task 1: weighted-centroid line position and motor-command limiting."""
SENSOR_WEIGHTS = (-2, -1, 0, 1, 2)

def estimate_line_position(readings):
    """Return centroid position, or None when all five readings are zero."""
    if len(readings) != 5:
        raise ValueError("Exactly five sensor readings are required")
    if any(value < 0 for value in readings):
        raise ValueError("Sensor readings must be non-negative")
    total = sum(readings)
    if total == 0:
        return None
    return sum(w * r for w, r in zip(SENSOR_WEIGHTS, readings)) / total

def limit_motor_commands(left, right, maximum):
    """Clamp signed commands to [-maximum, maximum], preserving sign."""
    if maximum < 0:
        raise ValueError("Maximum magnitude must be non-negative")
    return max(-maximum, min(left, maximum)), max(-maximum, min(right, maximum))

if __name__ == "__main__":
    for label, values in [
        ("centre", [0, 100, 300, 100, 0]),
        ("left", [300, 150, 0, 0, 0]),
        ("right", [0, 0, 0, 150, 300]),
        ("lost", [0, 0, 0, 0, 0]),
    ]:
        print(f"{label}: {estimate_line_position(values)}")
    print("clamped:", limit_motor_commands(310, -280, 255))
