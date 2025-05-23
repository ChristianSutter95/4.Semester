import unittest

from .Tree import Tree
from .Node import Node

class TestTree(unittest.TestCase):

    # creates for every test_* method an empty Tree
    def setUp(self):
        self.tree = Tree()

    # helper method for Tree structure (recursive)
    def structureTreeCorrect(self, node):
        if node.left:
            assert node.left.val < node.val, f"{node.left.val} should be < {node.val}"
            self.structureTreeCorrect(node.left)
        if node.right:
            assert node.right.val > node.val, f"{node.right.val} should be > {node.val}"
            self.structureTreeCorrect(node.right)

    # tests if the Tree is empty
    def test_emptyTree(self):
        self.assertTrue(self.tree.empty(), "Tree is not empty!")

    # insert one single node and tests if the Tree is empty and if the structure is correct
    def test_insert_single_node(self):
        self.tree.insert(Node(10))
        self.assertFalse(self.tree.empty(), "Tree is empty but should not be empty!")
        self.assertEqual(self.tree.getRoot().val, 10)
        node = self.tree.getItem(10)
        self.assertIsNotNone(node, f"Node with value {node.val} should be in Tree!")
        self.assertEqual(node.val, 10)

    # insert mutliple node and tests if the Tree is empty and if the structure is correct
    def test_insert_mutliple_nodes(self):
        # insert multiple nodes
        values = [10, 5, 15, 3, 7, 12, 18]
        for v in values:
            self.tree.insert(Node(v))

        # test if Nodes are correct inserted
        for v in values:
            node = self.tree.getItem(v)
            self.assertIsNotNone(node, f"Node with value {v} should be in Tree!")
            self.assertEqual(node.val, v)

        # test structure of the Tree
        self.structureTreeCorrect(self.tree.getRoot())

    def test_insert_two_Nodes_with_same_Value(self):
        values = [10, 10]
        for v in values:
            self.tree.insert(Node(v))

        # test if Nodes are correct inserted
        for v in values:
            node = self.tree.getItem(v)
            self.assertIsNotNone(node, f"Node with value {v} should be in Tree!")
            self.assertEqual(node.val, v)

        # test duplicates with child Nodes
        self.assertIsNotNone(self.tree.getItem(10), f"Node with value {v} should be in Tree!")
        self.assertIsNone(self.tree.getItem(10).left, "Root should not have any Childs!")
        self.assertIsNone(self.tree.getItem(10).right, "Root should not have any Childs!")

    def test_insert_multiple_Nodes_with_same_Value(self):
        values = [10, 5, 15, 3, 15, 7, 18, 12, 18, 3]
        for v in values:
            self.tree.insert(Node(v))

        # test if Nodes are correct inserted
        for v in values:
            node = self.tree.getItem(v)
            self.assertIsNotNone(node, f"Node with value {v} should be in Tree!")
            self.assertEqual(node.val, v)

        # test duplicates with the height of the tree
        self.assertEqual(self.tree.getHeight(self.tree.getRoot()), 3, "Tree should have a height of 3!")
