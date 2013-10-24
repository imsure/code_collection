#!/usr//bin/env python

"""
VectorSpace class provides the basic operations
for our vector-space-model based IR system.

Author: Shuo Yang
"""
import math

class VectorSpace(object):
    """
    This is the base class of the system which is
    responsible of building the vector space model for
    the IR system. It should be inherited by 'IR' class.

    Main responsibilites:
    * Map each term to its inverted document frequency.
    * Map each document or query to its weight vector.
      (normalized and not normalized)
    * Map each document or query to its vector length.
    * Compute similarities between a user qery and each documents.
    * Give back ranked query results from best match to worst match.
    """
    def __init__(self):
        # Do nothing.
        pass

    def buildVectorSpace(self, docs, docNum):
        """
        Build vector space for the system.

        Args:
        docs --> a list of 'Doc' object, represent
                 the whole document collection.
        docNum --> number of documents the system have,
                   used to compute inversed document frequency.

        Returns:
        t2idf --> {term, inversed_document_frequency} dictionary.
        docWeight --> {doc_name, weight_for_each_term_appeared_in_t2idf} dictionary.
        normalizedDocWeight --> {doc_name, normalized_weight_for_each_term} dictionary.
        docLength --> {doc_name, vector_length_computed_from_docWeight} dictionary.
        Note: the weight squence in 'docWeight' follows the same sequence of term
              in 't2idf' so that we don't need to store each term name for its weight
              in 'docWeight'.
        """
        t2idf = self.term2InvertedDocFreq(docs, docNum)
        
        # Map document name to its weight vector. (for print)
        docWeights = {}
        for doc in docs:
            docWeights[doc.getDocName()] = self.computeWeight(doc, t2idf)
            
        # Map document name to its vector length.
        docLengths = {}
        for dname in docWeights.iterkeys():
            docLengths[dname] = self.computeVecLen(docWeights[dname])

        # Map document name to its normalized weight vector. (for calculate similarity)
        normalizedDocWeights = {}
        for dname in docWeights.iterkeys():
            normalizedDocWeights[dname] = \
                self.normalizeWeights(docWeights[dname], docLengths[dname])

        return t2idf, docWeights, normalizedDocWeights, docLengths

    def computeVecLen(self, weights):
        """
        Compute the vector lenght of a given vector 'termWeights'
        which contains a list of (term, weight) tuple for a document
        or a user query.
        Formula:
        square_root_of(sum of each term's square weight)

        Args:
        termWeights --> term weight vector
        Return:
        length of vector weights.
        """
        # Extract weight elements from 'termWeights' to form a list.
        # weights = [w for t,w in termWeights]
        square = map(lambda x : x**2, weights)
        return math.sqrt(reduce(lambda x, y : x+y, square))
            
    def term2InvertedDocFreq(self, docs, docNum):
        """
        Associate each term in the system to its
        inverted document frequency

        Args:
        docs --> a list of 'Doc' object, represent
                 the whole document collection.
        Returns:
        term2docFreq --> list of (term, docFreq) tuples.
        (Note: do not use dictionary because it is naturally
               unsortable.)
        """
        t2idf = {}
        t2idf_sorted = []
        for doc in docs:
            for term in doc.getTermFreq():
                t2idf[term] = \
                    t2idf.get(term, 0) + 1
        for term in t2idf.iterkeys():
            t2idf[term] = math.log(float(docNum)/t2idf[term], 2)

        # Sort the dictionary by key (term).
        termlist = t2idf.keys()
        termlist.sort()
        for term in termlist:
            t2idf_sorted.append((term, t2idf[term]))

        return t2idf_sorted
                
    def computeWeight(self, obj, t2idf):
        """
        Compute tf-idf weight for the 'obj'.
        'obj' can be either a document or a user query.

        Return: weight vector of 'obj'
        """
        tf = obj.getTermFreq()
        weight = []

        for term, idf in t2idf:
            if tf.has_key(term):
                weight.append(tf[term]*idf)
            else:
                weight.append(0)
        return weight

    def normalizeWeights(self, weights, length):
        """
        Normalize term weight by dividing it by vector's length.
        This is done for further similarity calculation because we don't
        want to perform division each time a query enters in.

        Args:
        weights --> list of weight
        lengths --> vector_length dictionary.
        
        Returns:
        normalized weight vector.
        """
        return map(lambda x : x/length, weights)

    def getNewTerms(self, query_terms):
        """
        Get new terms from query that doesn't exist in
        the current vector space.
        """
        new_terms = []
        # Convert a list of tuples to a dictionary
        # for looking up.
        t2idf = dict(self.t2idf)
        for term in query_terms:
            if not t2idf.has_key(term):
                new_terms.append(term)
        return new_terms

    def updateVectorSpace(self, new_terms):
        """
        Add 'new_terms' to the current vector space.
        """
        ## idf is the same for all the new terms coming from query.
        idf = math.log(float(self.docNum)/0.5, 2)
        for term in new_terms:
            self.t2idf.append((term, idf))

        # Map document name to its weight vector. (for print)
        for doc in self.docs:
            self.docWeights[doc.getDocName()] = self.computeWeight(doc, self.t2idf)
            
        # Map document name to its vector length.
        for dname in self.docWeights.iterkeys():
            self.docLengths[dname] = self.computeVecLen(self.docWeights[dname])

        # Map document name to its normalized weight vector. (for calculate similarity)
        for dname in self.docWeights.iterkeys():
            self.normalizedDocWeights[dname] = \
                self.normalizeWeights(self.docWeights[dname], self.docLengths[dname])


    def processQuery(self, query):
        """
        Process a user query.

        Args:
        query --> a 'Query' object.
        Return:
        ranked results.
        """
        # Check if user query contains terms that do not
        # exist in the current vector space and get the
        # new term list.
        new_terms = self.getNewTerms(query.getTerms())
        if new_terms:
            print 'FIND NEW TERMS:', new_terms
            # will implement it later.
            self.updateVectorSpace(new_terms)

        query_weights = self.computeWeight(query, self.t2idf)
        query_len = self.computeVecLen(query_weights)
        query_weights_normalized = self.normalizeWeights(query_weights, query_len)

        return self.computeSimilarity(self.normalizedDocWeights,
                                      query_weights_normalized)

    def computeSimilarity(self, d_weights, q_weight):
        """
        Compute vector similarity between 'query' with each doc in 'docs'.
        Formula:
        d_weights[index] * q_weight
        
        Args:
        d_weights --> {doc_name, normalized_document_weight_vector} dictionary.
        q_weights --> normalized query weight vector
        (Both should be normalized)

        Return:
        [(doc_name, similarity)] list of tuples sorted by similarity.
        (Note: rank of each document should be the index number it
        correspond to.)
        """
        sims = []
        for doc_name, weight in d_weights.iteritems():
            sims.append((doc_name, self.dotProduct(weight, q_weight)))

        return sorted(sims, key=lambda sim : sim[1], reverse=True)

    def dotProduct(self, v1, v2):
        """
        Compute the dot product of two vectors
        v1 and v2 then return the result.
        """
        return sum([x*y for x,y in zip(v1, v2)])
        
