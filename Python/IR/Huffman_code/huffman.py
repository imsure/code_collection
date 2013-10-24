#!/usr/bin/env python

"""
This code implements Huffman compression algorithm.

The based idea is:
* Treat words and punctuations as encoding symble.
* Build a binary tree for encoding and decoding.
* Treat each symble as a singleton tree(one node, no branches)
* Use a priority queue to hold collection of trees.

The algorithm is:
1. Build a priority queue that contains a collection of singleton trees
   with weight equal to symbol frequency. Priority is based on weight.
2. Pick up two trees with the smallest frequencies and combine them as
   a new tree. Then insert the new tree into the priority queue.
3. Repeat step 3 until there is only one tree left in the queue.
   That's the final tree.
"""

import string
import re
from collections import Counter
from operator import itemgetter
import heapq
import pydot

DEBUG = True

text = "row row row your boat gently down your stream"

class Tree(object):
    """
    Represent a Huffman tree.
    Can be either a singleton tree or an multi-level tree.
    For singleton tree, 'left' and 'right' branch should be None.
    For multi-level tree, 'symbol' should be None.
    """
    def __init__(self, freq, symbol=None, name=None,
                 left_branch=None, right_branch=None):
        self.freq = freq
        self.symbol = symbol
        self.left = left_branch
        self.right = right_branch
        if symbol is not None: ## leaf
            self.name = symbol
        else: ## internal node
            self.name = name

    def isLeaf(self):
        return self.left == None and self.right == None

    def getFreq(self):
        return self.freq

    def getSymbol(self):
        return self.symbol

    def getLeftBranch(self):
        return self.left

    def getRightBranch(self):
        return self.right

    def printTree(self):
        if self.isLeaf():
            print self.freq, self.symbol
        else:
            self.left.printTree()
            print self.freq, self.symbol
            self.right.printTree()

    def node(self):
        """
        Return a pydot node for visualizing tree.
        """
        return pydot.Node(self.name)

table = {}
def Encode(root, prefix=[]):
    """
    Think recursively:
    * 
    """
    if root.isLeaf():
        table[root.getSymbol()] = list(prefix)
    else:
        prefix.append('0')
        Encode(root.getLeftBranch(), prefix)
        prefix.pop()
        prefix.append('1')
        Encode(root.getRightBranch(), prefix)
        prefix.pop()

def PrintTree(root):
    """
    Print a huffman tree in left-middle-right order.
    """
    if root is not None:
        PrintTree(root.getLeftBranch())
        print root.getFreq(), root.getSymbol()
        PrintTree(root.getRightBranch())

def VisualizeHuffmanTree(root, graph):
    if root.isLeaf():
        leaf = root.node()
        print root.name
        return leaf
    else:
        parent = root.node()
        left = VisualizeHuffmanTree(root.getLeftBranch(), graph)
        right = VisualizeHuffmanTree(root.getRightBranch(), graph)
        
        graph.add_node(parent)
        graph.add_node(left)
        graph.add_node(right)

        graph.add_edge(pydot.Edge(parent, left, label='0'))
        graph.add_edge(pydot.Edge(parent, right, label='1'))
        return parent

def ComputeSymbFreq(content):
    symbols = re.findall(r"[\w']+|[.,;:!?]", content)
    # list of (symble, frequency) tuples
    return Counter(symbols).items(), len(symbols)
    # return [(f, s) for (s, f) in Counter(symbols).items()]

def MakeLeaves(sym_freq):
    """
    Make a singleton tree for each symbol and

    Args:
    sym_freq --> list of (symbol, freq) tuples

    Returns:
    a list of singleton tree
    """
    return map(lambda sf : Tree(sf[1], sf[0]), sym_freq)

def MakeHuffmanTree(pqueue):
    """
    Args:
    pqueue --> a priority queue contains (freq, tree) tuples
               (lowest frequence has highest priority)

    Return:
    root of Huffman tree
    """
    ## Loop until there is only one tree left in the queue.
    loop_counter = 0
    while len(pqueue) > 1:
        t1 = heapq.heappop(pqueue)[1]
        t2 = heapq.heappop(pqueue)[1]
        loop_counter += 1
        if DEBUG:
            print '\niteration', loop_counter
            print 'smallest tree-1:',
            print 'freq:', t1.getFreq(), 'symbol:', t1.getSymbol()
            print 'smallest tree-2:',
            print 'freq:', t2.getFreq(), 'symbol:', t2.getSymbol()

        freq_sum = t1.getFreq() + t2.getFreq()
        new_tree = Tree(freq_sum, name='Node'+str(loop_counter), left_branch=t1, right_branch=t2)
        if DEBUG:
            print 'together form a new tree:',
            print 'freq:', new_tree.getFreq(), 'symbol:', new_tree.getSymbol()
        
        ## Push back the tree to the priority queue.
        heapq.heappush(pqueue, (freq_sum, new_tree))

    ## We are done. Pop up the final tree in the queue.
    return heapq.heappop(pqueue)[1]


def main():
    ## Compute symbol frequency.
    sym_freq, total_count = ComputeSymbFreq(text)
    if DEBUG:
        print '\nsymbol-frequency:'
        print sym_freq
        print 'total symbol count:', total_count

    sym_freq = AddPseudoEOF(sym_freq)

    ## Make leaves(singleton trees) for Huffman tree.
    singleton_trees = MakeLeaves(sym_freq)

    ## Priority queue. Hold (frequency, tree) tuples.
    ## Priority is based on frequency,
    ## lowest frequence has highest priority.
    pqueue = []
    map(lambda tree : heapq.heappush(pqueue, (tree.getFreq(), tree)),
        singleton_trees)

    huffman_tree = MakeHuffmanTree(pqueue)
    if huffman_tree.getFreq() == total_count:
        print '\nweight of final tree',
        print 'equals with the total symbol count', total_count
        print

    huffman_tree.printTree()
    Encode(huffman_tree)
    for sym, code in table.iteritems():
        print sym, ':', ''.join(table[sym])

    graph = pydot.Dot(graph_type='graph')
    VisualizeHuffmanTree(huffman_tree, graph)
    graph.write_png('tree.png')
    
if __name__ == '__main__':
    main()
