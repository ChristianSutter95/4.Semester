class Node:

    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

    # insert a Node in the correct Spot
    # Duplicates are not allowed
    def insert(self, node):
        if (self.val > node.val):
            if (self.left is None):
                self.left = node
            else:
                self.left.insert(node)
        elif (self.val < node.val):
            if (self.right is None):
                self.right = node
            else:
                self.right.insert(node)
        else:
            pass
