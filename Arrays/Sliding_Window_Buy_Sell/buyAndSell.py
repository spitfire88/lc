from typing import List
import random

class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        l, r = 0, 1 # l is buy, r sell
        maxP = 0

        while r < len(prices):
            if prices[l] < prices[r]:
                profit = prices[r] - prices[l]
                maxP = max(maxP, profit)
            else:
                l = r # move l ptr to lower price
            r += 1
        
        return maxP
    
solution = Solution()

prices = list(range(0, 10))
prices.extend([random.choice(prices) for _ in range(5)])
random.shuffle(prices)
prices = prices[-10:]
print(prices)
max_profit = solution.maxProfit(prices)
print(max_profit)