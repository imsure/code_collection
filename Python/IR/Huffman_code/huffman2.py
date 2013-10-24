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

import os
import string
import re
from operator import itemgetter
import heapq
## This module provides an object type which
## efficiently represents an array of booleans.
## url: http://pypi.python.org/pypi/bitarray/0.8.0
from bitarray import bitarray
## a Python interface to Graphviz's Dot language.
## url: http://code.google.com/p/pydot/
import pydot

DEBUG = True

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

def Encode(tree, code_table, prefix=[]):
    """
    Encode a huffman tree.

    Args:
    tree --> a huffman tree.
    code_table --> {symbol, code} dictionary for storing the coding result.
    prefix --> '0' and '1' sequence used for coding.

    Return: None
    """
    if tree.isLeaf():
        code_table[tree.getSymbol()] = list(prefix)
    else:
        prefix.append('0')
        Encode(tree.getLeftBranch(), code_table, prefix)
        prefix.pop()
        prefix.append('1')
        Encode(tree.getRightBranch(), code_table, prefix)
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
    d = {}
    for s in symbols:
        d[s] = d.get(s, 0) + 1
    return symbols, d.items()
    
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
    print 
    print '*' * 20,
    print 'Beginning of encoding:',
    print '*' * 20
    ## Loop until there is only one tree left in the queue.
    loop_counter = 0
    while len(pqueue) > 1:
        t1 = heapq.heappop(pqueue)[1]
        t2 = heapq.heappop(pqueue)[1]
        loop_counter += 1
        if DEBUG:
            print '\niteration', loop_counter
            print 'tree-1(',
            print 'freq:', t1.getFreq(), ', symbol:', t1.getSymbol(), ')'
            print '+ tree-2(',
            print 'freq:', t2.getFreq(), ', symbol:', t2.getSymbol(), ')'

        freq_sum = t1.getFreq() + t2.getFreq()
        new_tree = Tree(freq_sum, name='Node'+str(loop_counter), left_branch=t1, right_branch=t2)
        if DEBUG:
            print '-->',
            print 'new tree(',
            print 'freq:', new_tree.getFreq(), ', symbol:', new_tree.getSymbol(), ')'
        
        ## Push back the tree to the priority queue.
        heapq.heappush(pqueue, (freq_sum, new_tree))

    print
    print '*' * 20,
    print 'End of encoding:',
    print '*' * 20

    ## We are done. Pop up the final tree in the queue.
    return heapq.heappop(pqueue)[1]

def Convert2BitArray(symbols, code_table):
    """
    Convert from string representation of code to bit array.
    """
    barray = bitarray(endian='little')
    for symbol in symbols:
        for bit in code_table[symbol]:
            if bit == '1':
                barray.append(True)
            else: ## '0'
                barray.append(False)
    return barray

def Decode(barray, htree):
    """
    Decode bit array 'barray' given huffman coding
    represented by a huffman tree 'htree'.

    Return decoded content as a string.
    """
    if len(barray) == 0:
        print '*' * 20,
        print 'End of decoding:',
        print '*' * 20
        return ''

    content = ''
    ## Make a backup for the root of the tree.
    branch = htree
    symbol = ''
    for index, bit in enumerate(barray):
        if bit == True:
            branch = branch.getRightBranch()
        else:
            branch = branch.getLeftBranch()
            
        if branch.isLeaf():
            symbol = branch.getSymbol()
            break

    if symbol in '.,;:!?':
        content += symbol
    else:
        content += symbol + ' '

    print 'decode', barray[0:index+1],
    print '-->', symbol

    content += Decode(barray[index+1 : ], htree)
    return content


def main():
    ## Compute symbol frequency.
    fr = open('original_file', 'r')
    text = fr.read()
    symbols, sym_freq = ComputeSymbFreq(text)
    if DEBUG:
        print 'symbols:'
        print symbols
        print 'original file size:',
        print os.stat('./original_file').st_size, 'bytes'
        print '\nsymbol-frequency:'
        print sym_freq

    ## Implement it later.
    # sym_freq = AddPseudoEOF(sym_freq)

    ## Make leaves(singleton trees) for Huffman tree.
    singleton_trees = MakeLeaves(sym_freq)

    ## Priority queue. Hold (frequency, tree) tuples.
    ## Priority is based on frequency,
    ## lowest frequence has highest priority.
    pqueue = []
    map(lambda tree : heapq.heappush(pqueue, (tree.getFreq(), tree)),
        singleton_trees)

    huffman_tree = MakeHuffmanTree(pqueue)

    # huffman_tree.printTree()
    code_table = {}
    Encode(huffman_tree, code_table)
    print '\ncode table:'
    for sym, code in code_table.iteritems():
        print sym, '-->', ''.join(code_table[sym])

    ## Generate bit array for huffman code.
    barray = Convert2BitArray(symbols, code_table)
    print '\nbit array representation of the file:'
    print barray
    fw = open('compressed_file', 'w')
    barray.tofile(fw)
    fw.close()
    print 'compressed file size:',
    print os.stat('compressed_file').st_size, 'bytes'

    print
    print '*' * 20,
    print 'Beginning of decoding:',
    print '*' * 20
    decompressed_content = Decode(barray, huffman_tree)
    print '\ncontent after decompressing:'
    print decompressed_content

    graph = pydot.Dot(graph_type='graph')
    VisualizeHuffmanTree(huffman_tree, graph)
    graph.write_png('tree.png')
    
if __name__ == '__main__':
    main()


"""
Output:

symbols:
['row', 'row', 'row', 'your', 'boat', 'gently', 'down', 'your', 'stream']
original file size: 45 bytes

symbol-frequency:
[('stream', 1), ('gently', 1), ('down', 1), ('your', 2), ('boat', 1), ('row', 3)]

******************** Beginning of encoding: ********************

iteration 1
tree-1( freq: 1 , symbol: stream )
+ tree-2( freq: 1 , symbol: gently )
--> new tree( freq: 2 , symbol: None )

iteration 2
tree-1( freq: 1 , symbol: down )
+ tree-2( freq: 1 , symbol: boat )
--> new tree( freq: 2 , symbol: None )

iteration 3
tree-1( freq: 2 , symbol: your )
+ tree-2( freq: 2 , symbol: None )
--> new tree( freq: 4 , symbol: None )

iteration 4
tree-1( freq: 2 , symbol: None )
+ tree-2( freq: 3 , symbol: row )
--> new tree( freq: 5 , symbol: None )

iteration 5
tree-1( freq: 4 , symbol: None )
+ tree-2( freq: 5 , symbol: None )
--> new tree( freq: 9 , symbol: None )

******************** End of encoding: ********************

code table:
stream --> 010
gently --> 011
down --> 100
your --> 00
boat --> 101
row --> 11

bit array representation of the file:
bitarray('1111110010101110000010')
compressed file size: 3 bytes

******************** Beginning of decoding: ********************
decode bitarray('11') --> row
decode bitarray('11') --> row
decode bitarray('11') --> row
decode bitarray('00') --> your
decode bitarray('101') --> boat
decode bitarray('011') --> gently
decode bitarray('100') --> down
decode bitarray('00') --> your
decode bitarray('010') --> stream
******************** End of decoding: ********************

content after decompressing:
row row row your boat gently down your stream

"""
