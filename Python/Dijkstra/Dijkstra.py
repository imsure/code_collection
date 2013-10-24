#!/usr/bin/env python

"""
Implementation of Dijkstra's algorithm.

Dijkstra's algorithm operates by maintaining a subset of
vertices 'S' of the whole set of vertex 'V',
for which we know the true distance
d[v] = (shortest distance between source vertex 's' and 'v').

Intially 'S' = empty set. And we set d[s] = 0 and d[v] = -1 for
all others vertices. One by one we select vertices from 'V' to 'S'.

The set 'S' is implemented by an array of vertex colors.
Intially all vertices are 'white', and we set color[v] = 'black'
to indicate that 'v' belongs to 'S'.

Dijkstra's algorithm is a greedy algorithm(in order to make sure
we can always find a shortest path or shortest path tree).
For each vertex 'u' belongs to 'V'\'S'(means 'V'-'S', that is,
the subset of vertices we haven't processed), we have computed a
distance estimate d[u]. The next vertex processed is always a
vertex for which d[u] is minimum(For the very first time, 'u' is
source vertex 's'), that is, we take the unprocessed vertex that
is closest(by our estimate) to 's'.

"""

import sys
from operator import itemgetter
from Graph import *
from heapq import *

class Dijkstra(object):
    """
    Steps:
    1. Intilialize the source vertex as solved.
       Lable it with '0', since distance to itself is 0.
    2. Identify all unsolved vertices connected to any solved vertex.
    3. For each edge connecting a solved vertex and unsolved vertex,
       calculate the candidate distance.
       candidate distance = distance to the solved vertex + length of edge.
    4. Choose the smallest candidate distance.
       Change it to solved and label it with candidate distance.
    5. Add the edge to the edge set.
    6. Repeat step 2-5 until we have reached the destination vertex.
    
    """
    def __init__(self, graph, src_v, dest_v):
        """
        'graph' is a Graph object defined in Graph.py.
        'src_v' is the source vertex, take vertex 'O' as default.
        'dest_v' is the destination vertex, take vertex 'T' as default.
        """
        ## We use a dict to map each vertex 'v' to its distance 'd'
        ## where each v belongs to 'V'\'S'(subset of vertices not processed)
        self.v2d = {}

        self.graph = graph
        self.src_v = src_v
        self.dest_v = dest_v

        self.solved_vs = []

        ## keep track of the shortest path.
        self.previous = {}
        
        self.init_v2d()

    def init_v2d(self):
        """
        Initialize the priority queue. Set distance for source vertex
        as zero, and 'sys.maxint' for all other vertices.
        """
        for v in self.graph.keys():
            if v == self.src_v:
                self.v2d[v] = 0
            else:
                self.v2d[v] = sys.maxint

    def update_v2d(self, v, d):
        for key in self.v2d.keys():
            if key == v:
                self.v2d[v] = d

    def traverse(self):
        while self.v2d:
            ## pop up a vertex 'u' with shortest distance to source vertex.
            u, dist_u = sorted(self.v2d.iteritems(), key=itemgetter(1))[0]
            
            if dist_u == sys.maxint:
                print 'No vertex has the true distance.'
                break

            if u == self.dest_v:
                print '\nShortes path from %s to %s has been found' % (self.src_v, self.dest_v)
                print 'Shortest distance is', dist_u
                print
                self.shortest_path()
                break

            ## Pop up 'u'.
            for key in self.v2d.keys():
                if key == u:
                    self.v2d.pop(key)

            neighbor_vs = self.graph.find_neighbors(u)
            for v in neighbor_vs:
                if v in self.solved_vs:
                    continue
                
                alt_dist = dist_u + self.graph.dist_between(u, v)
                if alt_dist < self.v2d[v]:
                    self.update_v2d(v, alt_dist)
                    self.previous[v] = u

            self.solved_vs.append(u)

    def shortest_path(self):
        path = []
        v = self.graph.get_vertex(self.dest_v)

        path.append(v)
        while self.previous.has_key(v):
            path.append(self.previous[v])
            v = self.previous[v]

        path.reverse()
        print 'The shortest path:'
        print ' ==> '.join(map(str, path))
        print

def main():
    vs, es = load_graph()
    g = Graph(vs, es)

##    dk = Dijkstra(g, Vertex('O'), Vertex('T'))
    dk = Dijkstra(g, Vertex('E'), Vertex('A'))
    dk.traverse()

if __name__ == '__main__':
    main()
