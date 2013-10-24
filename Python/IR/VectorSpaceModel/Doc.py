#!/usr/bin/env python

"""
Doc.py is responsible for processing a single document
of the IR system and represent it as a 'Doc' object.

Author: Shuo Yang
"""

import os
import re
from operator import itemgetter
from IR import DEBUG
from utility import getTerms

class Doc(object):
    """
    This class represent a document in our IR system.

    Main Responsibilities:
    * Open a specific document file and get all terms and map
      each term to its normalized document frequency.
    """
    def __init__(self, docName, location='.', srcType='local'):
        self.name = docName
        if srcType == 'local':
            try:
                fp = open(os.path.join(location, docName), 'r')
            except IOError:
                print 'cannot open file', docName
            else:
                self.rawTerms = getTerms(fp.read())
  
    def getDocName(self):
        return self.name

    def maxTermFreq(self):
        maxTermFreq = \
            max(self.t2f.iteritems(), key=itemgetter(1))
        if DEBUG:
            print 'max term frequency for', self.getDocName(), 'is'
            print maxTermFreq
        # Build-in function max() will return the (term, freq) tuple,
        # we only want to return frequency.
        return maxTermFreq[1]

    def term2Freq(self):
        """
        Map each term to its frequency (how many times
        it occurs in the document)
        """
        self.t2f = {}
        for term in self.rawTerms:
            self.t2f[term] = self.t2f.get(term, 0) + 1
        self.normalizeTF(self.maxTermFreq())
        if DEBUG:
            print 'normalized term-frequency:'
            print self.t2f

    def normalizeTF(self, maxTF):
        """
        Normalize term frequency by dividing raw term count
        by max term frequency ('maxTF').
        """
        for term in self.t2f.iterkeys():
            self.t2f[term] = float(self.t2f[term])/maxTF

    def getTermFreq(self):
        return self.t2f

def processDocs(docNames, location='.', srcType='local'):
    """
    A wrapper function for class 'Doc'.
    It takes a list of document names and location and type
    information as arguments and returns a list of 'Doc' objects.
    """
    docs = []
    for name in docNames:
        doc = Doc(name, location, srcType)
        doc.term2Freq()
        docs.append(doc)

    return docs

