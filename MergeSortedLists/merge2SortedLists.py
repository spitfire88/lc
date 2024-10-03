import random

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

    def create_sorted_list(nums):
        head = ListNode()
        curr = head
        for num in sorted(nums):
            curr.next = ListNode(num)
            curr = curr.next
        return head.next
    
    def merge_sorted_lists(l1, l2):
        dummy = ListNode()
        curr = dummy
        while l1 and l2:
            if l1.val < l2.val:
                curr.next = l1
                l1 = l1.next
            else:
                curr.next = l2
                l2 = l2.next
            curr = curr.next
        return dummy.next
    
# Create a random array of 10 numbers ranging from 0 to 1000
n1 = [random.randint(0, 1000) for _ in range(10)]
n2 = [random.randint(0, 1000) for _ in range(10)]

# Create sorted linked lists from the random array
l1 = ListNode.create_sorted_list(n1)
l2 = ListNode.create_sorted_list(n2)

# Merge the sorted linked lists
merged_list = ListNode.merge_sorted_lists(l1, l2)

# Print the merged list
while merged_list:
    print(merged_list.val, end='\t')
    merged_list = merged_list.next
print()