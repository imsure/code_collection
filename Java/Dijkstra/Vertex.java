/*
 * Class Vertex represents nodes in an undirected graph.
 */

public class Vertex {
    private String label = "";
    private int dist; // shortest distance to the start vertex.
	
    public Vertex(String label) {
	this.label = label;
	dist = Integer.MAX_VALUE; // Initialize to maximum value at first.
    }

    public int distToStart() {
	return this.dist;
    }

    public void setDistToStart(int new_val) {
	this.dist = new_val;
    }

    public void printSelf() {
	System.out.println(label);
    }
	
    public String getLabel() {
	return label;
    }

    public boolean cmp(Vertex v) {
	return this.label.equals(v.getLabel());
    }
}
