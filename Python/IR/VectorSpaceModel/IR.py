#!/usr/bin/env python

"""
A Simple Vector Space Model based Information Retrieval System.
Features:
* Only support processing local document collection.
  (must be ASCII English text format).
  Need to add support of different file formats
  and getting data from web.
* Everything is done on the fly (in memory), so not preferable
  to large collections.
  Need to add support for indexing and compression.
* Simply extract terms out of documents and convert to lowercase.
  No support of stemming and stop words.
* Treat a set of document as a set of vectors in a vector space,
  in which there is one axis for each term in the space.

Author: Shuo Yang
"""

import os
import Doc
from VectorSpace import VectorSpace
from pprint import pprint
from Query import Query

DEBUG = False

class IR(VectorSpace):
    """
    The core class of the IR system. It inherites the
    'VectorSpace' class and is in charge of the control
    flow of the system.
    
    Main Responsibilities:
    * Setup the system.
    * Process document collection.
    * Build vector space for the system.
    * Print out important system information.
    * Start user query and show ranked query results.
    """
    def __init__(self, **docOptions):
        # 'docInfo' contains the information about documents source.
        self.docInfo = {'srcType' : 'local',
                        'location' : '.',
                        'suffix' : '.txt'} 

        if 'srcType' in docOptions:
            self.docInfo['srcType'] = docOptions['srcType']
            
        if 'location' in docOptions:
            self.docInfo['location'] = docOptions['location']

        if 'suffix' in docOptions:
            self.docInfo['suffix'] = docOptions['suffix']

    def printSystemInfo(self):
        """
        Print out system information after being set up.
        """
        print '\nTHIS IS A SIMPLE VECTOR SPACE MODEL INFORMATION RETRIEVAL SYSTEM.\n'
        print 'Document source type:', self.docInfo['srcType']
        print 'Document location:', os.path.abspath(self.docInfo['location'])
        print
        
        dw = self.docWeights

        # Get sorted document name list.
        doc_names = [doc_name for doc_name in dw.iterkeys()]
        doc_names.sort()

        print '-' * 50
        print 'TERM-WEIGHT TABLE'
        print '(each column is sorted by document names)'
        print '(each row is sorted by terms)'
        print '-' * 50
        print
        
        print 'TERM'.ljust(10), ''.ljust(8).join(doc_names)
        print '-' * (11+9*4)

        # Get sorted term list.
        terms = [term for term, idf in self.t2idf]
        for index, term in enumerate(terms):
            # Get weight list for a given term.
            weights4term = map(lambda x : dw[x][index], doc_names)
            weights4term = map(lambda x : format(x, '.2f'), weights4term)
            print str(term).ljust(10), ''.ljust(10).join(map(str, weights4term))

        print '-' * (11+9*4)
        lengths = map(lambda x : self.docLengths[x], doc_names)
        lengths = map(lambda x : format(x, '.2f'), lengths)
        print 'LENGTH'.ljust(10), ''.ljust(10).join(map(str, lengths))

    def getDocList(self):
        src_type = self.docInfo['srcType']
        suffix = self.docInfo['suffix']
        suffix_len = len(suffix)
        loc = self.docInfo['location']

        # Define a temp function to valid file names.
        valid_file = lambda x : x[-suffix_len :] == suffix

        if src_type == 'local':
            # Filter out invalid files.
            return filter(valid_file, os.listdir(loc))
        elif src_type == 'web':
            # Leave it blank temporarily.
            return []
    
    def processCollection(self):
        """
        Process documents collection of the system.
        """
        # List of Document objects held by the system. 
        docs = []
        # Get list of document name.
        self.docNames = self.getDocList()
        self.docNum = len(self.docNames)
        if DEBUG:
            print 'document list:'
            print self.docNames

        return Doc.processDocs(self.docNames,
                               self.docInfo['location'],
                               self.docInfo['srcType'])

    def systemSetup(self):
        """
        Two major steps:
        1. Process document collection.
        2. Build vector space model for the system.
        """
        # Get a list of 'Doc' objects for building vector space model.
        self.docs = self.processCollection()
        self.t2idf, self.docWeights, self.normalizedDocWeights, self.docLengths = \
            self.buildVectorSpace(self.docs, self.docNum)

    def startUserQuery(self):
        while True:            
            query = Query()
            if query.isLeave():
                print '\nGoodbye!'
                break

            # rank is a list of tuple (doc_name, similarity)
            # sorted by similarity in decreasing order.
            rank = self.processQuery(query)
            self.printSystemInfo()
            print
            print 'RANK'.ljust(10), 'DOC'.ljust(10), 'SIMILARITY'
            for index, item in enumerate(rank):
                print str(index).ljust(10), item[0].ljust(10),
                print str(format(item[1], '.2f')).ljust(10)

def main():
    """
    Main entry of the system.
    """
    ir = IR(location='./docs')
    ir.systemSetup()
    ir.startUserQuery()
    
if __name__ == '__main__':
    main()
