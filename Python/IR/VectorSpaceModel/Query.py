#!/usr/bin/env python

"""
'Query' class represents a user query.

Author: Shuo Yang
"""
from utility import getTerms

class Query(object):
    """
    Query class simply represents a user input query.
    """
    def __init__(self):
        print '\nPlease enter a query:'
        self.query = raw_input()
        self.terms = getTerms(self.query)

    def getTerms(self):
        return self.terms

    def isLeave(self):
        return self.query == 'q'

    def getTermFreq(self):
        """
        For query, we just assume that its term frequencies
        are 1.
        """
        t2f = {}
        for term in self.terms:
            t2f[term] = 1
        return t2f
        
