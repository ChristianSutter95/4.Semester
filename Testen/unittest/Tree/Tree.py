from Node import Node
from exceptions import ItemNotFoundError

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
    
    # return true if the tree conatins a specific value (recursive)
    def lookup(self, val, node):
        if node is None:
            return False
        if node.val == val:
            return True
        elif val < node.val:
            return self.lookup(val, node.left)
        else:
            return self.lookup(val, node.right)

    # return the String of a Tree/SubTree
    def toString(self, node):
        if node is None:
            return ""
        return self.toString(node.left) + str(node.val) + self.toString(node.right)

    # remove a Node in the Tree and returns it
    def remove(self, val, node):
        if node is None:
            raise ItemNotFoundError(f"Value {val} not found in tree.")

        if val < node.val:
            node.left = self.remove(val, node.left)
        elif val > node.val:
            node.right = self.remove(val, node.right)
        else:
            if node.left is None and node.right is None:
                return None
            if node.left is None:
                return node.right
            if node.right is None:
                return node.left
            minLargerNode = self.findMin(node.right)
            node.val = minLargerNode.val
            node.right = self.remove(minLargerNode.val, node.right)

        return node


    def findMin(self, node):
        while node.left:
            node = node.left
        return node
