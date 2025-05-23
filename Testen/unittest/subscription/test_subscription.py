import unittest

from subscription import Subscription

class TestSubscription(unittest.TestCase):
    def test_1(self):
        s = Subscription(24.00, 2)
        self.assertEqual(s.pricePerMonth(), 12)

    def test_2(self):
        s = Subscription(27.30, 2)
        self.assertEqual(s.pricePerMonth(), 13.65)

 
    def test_3(self):
        s = Subscription(21.70, 3)
        self.assertEqual(s.pricePerMonth(), 7.24)
       
if __name__ == "__main__":
    unittest.main(verbosity=2)