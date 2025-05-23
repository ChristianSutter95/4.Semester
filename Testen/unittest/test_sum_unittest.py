# 1.
import unittest

# 2.
class TestSum(unittest.TestCase):

    # 3.
    def test_sum(self):
        # 4.
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

    # 3.
    def test_sum_tuple(self):
        # 4.
        self.assertEqual(sum((1, 2, 2)), 6, "Should be 6")

if __name__ == '__main__':
    # 5.
    unittest.main()

    