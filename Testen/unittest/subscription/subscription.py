import math

class Subscription:
    
    def __init__(self, price, num):
        """
        Constructor

        Parameters
        ----------
	price : total price for a subcription in euro
	num : length of subcription in months 
        """
        self.price = price
        self.nofMonth = num
    	
    def pricePerMonth(self):
        """
        Calculates the monthly subscription price rounded up to the 
        nearest cent.

        Returns
        -------
        subscription price per month in euro
        """
        return math.ceil(self.price * 100 / self.nofMonth) / 100 
    	
    def cancel(self):
        """
        Call this to cancel/nullify this subscription
        """  
        self.nofMonth = 0