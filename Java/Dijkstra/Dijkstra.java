/*
 * Implementation of Dijkstra's algorithm for finding
 * the shortest path. Also this class is responsible
 * for providing interface to GUI programs.

 * Implementation details:
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
    
 */

import java.util.Iterator;
import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Comparator;
import java.util.Set;

public class Dijkstra {

    private Graph graph = null;
    private Vertex start = null;
    private Vertex end = null;

    // We use a priority queue to keep track of unsolved vertices.
    private PriorityQueue<Vertex> pqueue;
    // Used to store all solved vertice.
    private ArrayList<Vertex> solved;
    // Used to keep track of the shortest path.
    private Map<Vertex, Vertex> prev;

    public Dijkstra(Graph graph, Vertex start, Vertex end) {
	this.graph = graph;
	this.start = start;
	this.end = end;

	Comparator<Vertex> comparator = new DistComparator();
	pqueue = new PriorityQueue<Vertex>(graph.getVertexNum(), comparator);
	solved = new ArrayList<Vertex>();
	prev = new HashMap<Vertex, Vertex>();

	initPQueue();
    }

    /*
     * Initiliaze the priority queue 'pq'.
     */
    private void initPQueue() {
	Set<Vertex> vset = graph.getVertice();
	Iterator it = vset.iterator();

	while (it.hasNext()) {
	    Vertex v = (Vertex) it.next();
	    if (v.cmp(start)) {
		// Start vertex has distance of zero to itself.
		v.setDistToStart(0);
	    }
	    pqueue.add(v);
	}

	/*
	while (pqueue.size() != 0) {
	    System.out.println("Priority Queue:");
	    Vertex u = pqueue.remove();
	    System.out.println(u.getLabel() + " : " + u.distToStart());
	}
	*/
    }

    public void traverse() {

	while (!this.pqueue.isEmpty()) {
	    // Pop up the vertex with smallest distance
	    // to the start vertex.
	    Vertex u = this.pqueue.remove();
	    int dist = u.distToStart();

	    if (u.cmp(this.end)) {
		// If we have popped up the end vertex, we are done.
		System.out.println("Shortest path found!");
		System.out.println("Shortest distance is " + dist);
	    }

	    ArrayList<Vertex> neighbors = this.graph.findNeighbors(u);
	    for (int i = 0; i < neighbors.size(); i++) {
		Vertex v = neighbors.get(i);
		if (this.solved.contains(v)) {
		    // Ignore the vertex that has been solved.
		    continue;
		}

		int alt_dist = dist + this.graph.distBetween(u, v);
		if (alt_dist < v.distToStart()) {
		    this.pqueue.remove(v);
		    v.setDistToStart(alt_dist);
		    this.pqueue.add(v);
		    // Vertex 'u' is the previous vertex of 'v'.
		    // We need to keep track of it so that we can
		    // retrieve the final shortest path.
		    this.prev.put(v, u);
		}
	    }

	    // Updated arrayList of solved vertice.
	    this.solved.add(u);
	}
    }

    public void printPath() {
	ArrayList<Vertex> path = new ArrayList<Vertex>();
	Vertex v = this.graph.getVertex(this.end);

	path.add(v);
	while (this.prev.containsKey(v)) {
	    v = this.prev.get(v);
	    path.add(v);
	}

	for (int i = path.size()-1; i >= 0; i--) {
	    path.get(i).printSelf();
	}
    }
}

