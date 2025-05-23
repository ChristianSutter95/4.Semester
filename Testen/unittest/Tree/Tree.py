from .Node import Node

class Tree:
    
    def __init__(self):
        self.root = None

    # insert a node into the Tree
    def insert(self, node):
        if self.root is None:
            self.root = node
        else: 
            self.root.insert(node)

    # checks if the Tree is empty
    def empty(self):
        return self.root == None
    
    # return the root of the Tree
    def getRoot(self):
        return self.root
    
    # returns a Node with a specific value
    def getItem(self, val):
        node = self.root
        while node is not None and node.val != val:
            if val < node.val:
                node = node.left
            else:
                node = node.right
        return node
    
    # returns the height of the Tree (recursive)
    def getHeight(self, node):
        if node is None:
            return 0
        return 1 + max(self.getHeight(node.left), self.getHeight(node.right))