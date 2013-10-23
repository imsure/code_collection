/*
 * Undirected graph representation.
 */

import java.io.BufferedReader;
import java.util.Iterator;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;

public class Graph {
    /*
     * We use a hash map of hash map to represent a undirected graph.
     * The outer hash maps a vertex to an inner hash which maps from
     * other vertices to edges.
     */
    private HashMap outerHash;
    private int vnum = 0;
	
    public Graph() {
	outerHash = new HashMap();
    }
	
    private void addVertex(Vertex v) {
	//System.out.println("Putting " + v.getLabel() + " into hash map.");
	vnum++;
	outerHash.put(v, new HashMap());
    }
	
    private void addEdge(Vertex v1, Vertex v2, int dist) {
	Edge e = new Edge(v1, v2, dist);
		
	//System.out.println("Adding " + v1.getLabel() + " to " + v2.getLabel());
	HashMap innerHash = (HashMap) outerHash.get(v1);
	innerHash.put(v2, new Edge(v1, v2, dist));
		
	innerHash = (HashMap) outerHash.get(v2);
	innerHash.put(v1, new Edge(v2, v1, dist));
	//System.out.println("Adding edge done");
    }

    public int getVertexNum() {
	return vnum;
    }

    /*
     * Get a specific vertex hold by graph given the label 'label' of the vertex.
     */
    public Vertex getVertex(Vertex u) {
	Iterator it = outerHash.keySet().iterator();
		
	Vertex v = null;
	while (it.hasNext()) { 
	    v = (Vertex) it.next();
	    if (v.cmp(u)) {
		return v;
	    }
	}
	return null;
    }

    public Set<Vertex> getVertice() {
	return outerHash.keySet();
    }

    /*
     * Find all neighbors of a given vertex 'v' and return
     * an arraylist that contains them.
     */
    public ArrayList<Vertex> findNeighbors(Vertex v) {
	ArrayList<Vertex> vs = new ArrayList<Vertex>();

	HashMap inner = (HashMap) outerHash.get(getVertex(v));
	Iterator it = inner.keySet().iterator();

        while (it.hasNext()) {
            Vertex u = (Vertex) it.next();
	    vs.add(u);
	}
	return vs;
    }

    public int distBetween(Vertex u, Vertex v) {
	HashMap inner = (HashMap) outerHash.get(getVertex(u));
	Edge e = (Edge) inner.get(getVertex(v));
	return e.getDist();
    }

    public void loadGraphFromFile(String fname) {
	try {
	    FileInputStream fin = new FileInputStream(fname);
	    DataInputStream din = new DataInputStream(fin);
	    BufferedReader br = new BufferedReader(new InputStreamReader(din));
			
	    String line;
	    Map<String, Vertex> label2vertex = new HashMap<String, Vertex>();
	    while ((line = br.readLine()) != null) {
		if (line.length() == 0 || line.charAt(0) == '#')	
		    continue;
				
		String[] elems = line.split(":");
		if (!label2vertex.containsKey(elems[0])) {
		    Vertex v1 = new Vertex(elems[0]);
		    label2vertex.put(elems[0], v1);
		    addVertex(v1);
		}
		if (!label2vertex.containsKey(elems[1])) {
		    Vertex v2 = new Vertex(elems[1]);
		    label2vertex.put(elems[1], v2);
		    addVertex(v2);
		}
				
		addEdge(label2vertex.get(elems[0]), label2vertex.get(elems[1]), 
			Integer.parseInt(elems[2]));
	    }
			
	} catch (Exception e) {
	    System.err.println("Error: " + e.getMessage());
	}
    }
	
    public void printSelf() {
	Iterator it1 = outerHash.keySet().iterator();
		
	while (it1.hasNext()) { 
	    Vertex outer_v = (Vertex) it1.next();
	    HashMap inner_hash = (HashMap) outerHash.get(outer_v);
	    outer_v.printSelf(); 
			
	    Iterator it2 = inner_hash.keySet().iterator();
	    while (it2.hasNext()) {
		Vertex inner_v = (Vertex) it2.next();
		Edge e = (Edge) inner_hash.get(inner_v);
		System.out.println("\t" + inner_v.getLabel() + "\t" + e.getDist());
	    }
	}
    }
}
