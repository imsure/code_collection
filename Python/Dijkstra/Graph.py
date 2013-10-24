#!/usr/bin/env python

"""
Module represents a undirected graph.
"""

class Vertex(object):
    """A vertex is a node in a graph"""

    def __init__(self, label=''):
        self.label = label

    def __repr__(self):
        """Return a string representation of
        this object that can be evaluated as
        a python expression."""
        return 'Vertex(%s)' % repr(self.label)

    def __eq__(self, other_v):
        return self.label == other_v.label

    ## Two forms are the same for this object.
    __str__ = __repr__

    
class Edge(object):
    """An edge contains two vertices and distance
    between these two vertices"""
    def __init__(self, v1, v2, distance):
        ## Use a tuple to store two vertices.
        self.vs = (v1, v2)
        self.distance = distance

    def get_distance(self):
        return self.distance

    def __repr__(self):
        return 'Edge(%s, %s):%d' % (repr(self.vs[0]),
                                    repr(self.vs[1]),
                                    self.distance)

    def vertices(self):
        return self.vs

    __str__ = __repr__

class Graph(dict):
    """A graph is dictionary of dictionaries. The
    outer dictionary maps from a vertex to an inner
    dictionary which maps from other vertices to edges.

    (Thus it is better to let it inherit from class dict.)

    For vertices a and b, graph[a][b] maps to the edge
    that connects a --> b, if it exists."""

    def __init__(self, vs=[], es=[]):
        """Create a new graph.
        vs: list of vertices.
        es: list of edges.
        """
        for v in vs:
            self.add_vertex(v)

        for e in es:
            self.add_edge(e)

    def add_vertex(self, v):
        ## Initialize the outer dictionary.
        self[v] = {}
    
    def add_edge(self, e):
        v1, v2 = e.vertices()

        ## If there is already an edge connecting
        ## v1 and v2. The new edge replace it.
        self[v1][v2] = e
        self[v2][v1] = e

    def get_vertex(self, v):
        for u in self.keys():
            if u == v:
                return u

    def find_neighbors(self, v):
        """Find all neighbors of vertex 'v'."""
        return self[v].keys()

    def dist_between(self, u, v):
        """Return the distance between vertices 'u' and 'v'."""
        return self[u][v].get_distance()

    def __repr__(self):
        """Return a string representation of
        this object that can be evaluated as
        a python expression."""
        from cStringIO import StringIO
        import sys

        old_stdout = sys.stdout
        sys.stdout = mystdout = StringIO()

        for outer_v, inner_dict in self.iteritems():
            print '\nStart from:', outer_v
            for inner_v, e in inner_dict.iteritems():
                print outer_v, '--(%d)-->' % e.get_distance(), inner_v 

        sys.stdout = old_stdout
        return mystdout.getvalue()

    __str__ = __repr__

def load_graph(fname='network.txt'):
    """
    Load a graph from file called 'fname'.

    Return a list of vertices and a list of edges.
    """
    try:
        fp = open(fname, 'r')
    except IOError as e:
        print 'IO Error({0}, {1})'.format(e.errno, e.sterror)
        raise

    label_to_vertex = {}
    vs = []
    es = []
    
    for line in fp.readlines():
        if line.startswith('##') or line == '\n':
            ## Ignore blank lines or lines starting with '##'.
            continue
        
        items = line.split(':')
        label1, label2, distance = items[0], items[1], int(items[2])

        if not label_to_vertex.has_key(label1):
            label_to_vertex[label1] = Vertex(label1)
            vs.append(label_to_vertex[label1])

        if not label_to_vertex.has_key(label2):
            label_to_vertex[label2] = Vertex(label2)
            vs.append(label_to_vertex[label2])

        es.append(Edge(label_to_vertex[label1],
                       label_to_vertex[label2],
                       distance))

    return vs, es

def demo():
    v = Vertex('v')
    print v
    w = Vertex('w')
    print w
    e = Edge(v, w, 10)
    print e
    g = Graph([v,w], [e])
    print g
    print

def main(script, *args):
    vs, es = load_graph()
    g2 = Graph(vs, es)
    print g2


if __name__ == '__main__':
    import sys
    main(*sys.argv)
