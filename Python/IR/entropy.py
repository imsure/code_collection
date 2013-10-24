#!/usr/bin/env python

"""
Python code for computing entropy of text streams
as appeared in HW4.

Formula for computing entropy:
E = -sum_of(p(i) * log2p(i)) where
i is in range of [1, number of symbols]
"""

import os
from string import ascii_uppercase
from math import log

def entropyRandom(stream):
    """
    Compute the entropy of a random stream.
    Here random means that all the elements in
    the stream are equally distributed.

    Return the entropy of 'stream'
    """
    prob = 1.0 / len(stream)
    return -(prob * log(prob, 2)) * len(stream)

def entropyDistributed(distribution):
    """
    Compute the entropy of a stream in which each
    element has a different distribution.

    Args:
    distribution --> list of probabilities of a given stream.

    Return:
    entropy of a given stream.
    """
    return -sum(map(lambda p : p * log(p, 2), distribution))

if __name__ == '__main__':
    res1 = entropyRandom(ascii_uppercase)
    print 'entropy for a randomly distributed',
    print ascii_uppercase, 'is', res1

    f = open('EnglishLetterFrequencies.txt', 'r')
    lines = f.readlines()
    lines_split = map(lambda line : line.split(), lines)
    probs = map(lambda line : float(line[1]), lines_split)

    res2 = entropyDistributed(probs)
    print 'entropy for', ascii_uppercase,
    print 'with a given distribution is', res2
    

"""
Results:

imsure@ubuntu:~/Programming/py/IR$ ./entropy.py

entropy for a randomly distributed ABCDEFGHIJKLMNOPQRSTUVWXYZ is 4.70043971814

entropy for ABCDEFGHIJKLMNOPQRSTUVWXYZ with a given distribution is 4.25649663383

"""
