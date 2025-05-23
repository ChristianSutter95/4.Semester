import unittest

from Tree import Tree
from Node import Node
from exceptions import ItemNotFoundError

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

    # tests if a insert a duplicate Node is disallowed
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

     # tests if a insert a duplicate Node is disallowed
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

    # tests if the lookup works correctly in a Tree
    def test_lookup_value_in_whole_tree(self):
        values = [10, 5, 15, 3, 7, 12, 18]
        for v in values:
            self.tree.insert(Node(v))

        self.assertTrue(self.tree.lookup(7, self.tree.getRoot()))
        self.assertTrue(self.tree.lookup(12, self.tree.getRoot()))
        self.assertFalse(self.tree.lookup(99, self.tree.getRoot()))

    # tests if the lookup works correctly in a Subtree 
    def test_lookup_in_subtree(self):
        values = [10, 5, 15, 3, 7, 12, 18]
        for v in values:
            self.tree.insert(Node(v))

        leftSubTree = self.tree.getItem(5)
        rightSubTree = self.tree.getItem(15)

        self.assertTrue(self.tree.lookup(3, leftSubTree))
        self.assertTrue(self.tree.lookup(7, leftSubTree))
        self.assertFalse(self.tree.lookup(12, leftSubTree))
        self.assertTrue(self.tree.lookup(12, rightSubTree))
        self.assertFalse(self.tree.lookup(5, rightSubTree))
        
    # tests if the toString method works correctly with a full Tree
    def test_toString_entire_tree(self):
        values = [6, 2, 9, 3]
        for v in values:
            self.tree.insert(Node(v))
        result = self.tree.toString(self.tree.getRoot())
        self.assertEqual(result, "2369")

    # tests if the toString method works correctly with a single Node
    def test_toString_single_node(self):
        self.tree.insert(Node(42))
        result = self.tree.toString(self.tree.getRoot())
        self.assertEqual(result, "42")

    # tests if the toString method works correctly with a left heavy Tree
    def test_toString_left_heavy_tree(self):
        values = [5, 3, 2, 1]
        for v in values:
            self.tree.insert(Node(v))
        result = self.tree.toString(self.tree.getRoot())
        self.assertEqual(result, "1235")

    # tests if the toString method works correctly with a right heavy Tree
    def test_toString_right_heavy_tree(self):
        values = [5, 7, 8, 9]
        for v in values:
            self.tree.insert(Node(v))
        result = self.tree.toString(self.tree.getRoot())
        self.assertEqual(result, "5789")

    # tests if the toString method works correctly with a empty Tree
    def test_toString_empty_tree(self):
        result = self.tree.toString(self.tree.getRoot())  # root is None
        self.assertEqual(result, "")

    # tests if a leaf Node gets removed and if the Tree rebuild correct
    def test_remove_leaf_node(self):
        values = [10, 5, 15]
        for v in values:
            self.tree.insert(Node(v))
        
        self.tree.root = self.tree.remove(5, self.tree.getRoot())
        self.assertIsNone(self.tree.getItem(5))
        self.structureTreeCorrect(self.tree.getRoot())

    # tests if a Node with one Child gets removed and if the Tree rebuild correct
    def test_remove_node_with_one_child(self):
        values = [10, 5, 15, 12]
        for v in values:
            self.tree.insert(Node(v))
        self.tree.root = self.tree.remove(15, self.tree.getRoot())
        self.assertIsNone(self.tree.getItem(15))
        self.assertEqual(self.tree.getItem(12).val, 12)
        self.structureTreeCorrect(self.tree.getRoot())

    # tests if a Node with two Childs gets removed and if the Tree rebuild correct
    def test_remove_node_with_two_children(self):
        values = [10, 5, 15, 12, 18]
        for v in values:
            self.tree.insert(Node(v))
        self.tree.root = self.tree.remove(15, self.tree.getRoot())
        self.assertIsNone(self.tree.getItem(15))
        self.structureTreeCorrect(self.tree.getRoot())

    # tests if the Root gets removed and if the Tree rebuild correct
    def test_remove_root_node(self):
        values = [10, 5, 15, 12]
        for v in values:
            self.tree.insert(Node(v))
        self.tree.root = self.tree.remove(10, self.tree.getRoot())
        self.assertIsNone(self.tree.getItem(10))
        self.structureTreeCorrect(self.tree.getRoot())

    # tests if a Node which don't excist gets removed and if the Tree rebuild correct
    def test_remove_nonexistent_value_raises_exception(self):
        values = [10, 5, 15]
        for v in values:
            self.tree.insert(Node(v))
        with self.assertRaises(ItemNotFoundError):
            self.tree.root = self.tree.remove(99, self.tree.getRoot())


    def test_remove_node_with_only_right_child(self):
        self.tree.insert(Node(10))
        self.tree.insert(Node(20))
    
        self.tree.root = self.tree.remove(10, self.tree.getRoot())
    
        self.assertEqual(self.tree.getRoot().val, 20)
        self.assertIsNone(self.tree.getRoot().left)
        self.assertIsNone(self.tree.getRoot().right)
        self.structureTreeCorrect(self.tree.getRoot())
