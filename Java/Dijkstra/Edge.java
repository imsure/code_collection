/*
 * Class Edge represents an edge in an undirected graph.
 */

public class Edge {
	private Vertex[] vs;	// 2 vertices at each end of the edge.
	private int dist = 0;
	
	public Edge(Vertex v1, Vertex v2, int dist) {
		vs = new Vertex[2];
		vs[0] = v1;
		vs[1] = v2;
		this.dist = dist;
	}
	
	public int getDist() {
		return dist;
	}
	
	public void printSelf() {
		System.out.format("Edge(%s, %s) : %d\n", vs[0].getLabel(), vs[1].getLabel(), dist);
	}
}
