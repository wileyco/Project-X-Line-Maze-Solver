import unittest
from src.task1_basic_coding import estimate_line_position, limit_motor_commands

class Task1Tests(unittest.TestCase):
    def test_center(self):
        self.assertEqual(estimate_line_position([0, 100, 300, 100, 0]), 0.0)
    def test_left_negative(self):
        self.assertLess(estimate_line_position([300, 150, 0, 0, 0]), 0)
    def test_right_positive(self):
        self.assertGreater(estimate_line_position([0, 0, 0, 150, 300]), 0)
    def test_lost_line(self):
        self.assertIsNone(estimate_line_position([0, 0, 0, 0, 0]))
    def test_wrong_length(self):
        with self.assertRaises(ValueError):
            estimate_line_position([1, 2, 3])
    def test_negative_reading(self):
        with self.assertRaises(ValueError):
            estimate_line_position([0, 0, -1, 0, 0])
    def test_clamp_both_signs(self):
        self.assertEqual(limit_motor_commands(300, -300, 255), (255, -255))
    def test_within_limits(self):
        self.assertEqual(limit_motor_commands(100, -100, 255), (100, -100))
    def test_negative_limit_rejected(self):
        with self.assertRaises(ValueError):
            limit_motor_commands(1, 2, -1)

if __name__ == "__main__":
    unittest.main()
