/*
 * Entry point of the program.
 */

import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) {
	Vertex u = new Vertex("u");
	Vertex v = new Vertex("v");
	Edge e = new Edge(u, v, 5);
	e.printSelf();	
		
	Graph g = new Graph();
	g.loadGraphFromFile("network.txt");
	//g.printSelf();
	Vertex o = g.getVertex(new Vertex("O"));
	o.printSelf();

	ArrayList<Vertex> vs = g.findNeighbors(o);
	//System.out.print(vs);
	System.out.println("Neighbors:");
	for (int i = 0; i < vs.size(); i++) {
	    Vertex ver = vs.get(i);
	    ver.printSelf();
	    System.out.println("Distance: " + g.distBetween(o, vs.get(i)));
	}

	Dijkstra dij = new Dijkstra(g, new Vertex("O"), new Vertex("T"));
	dij.traverse();
	dij.printPath();
    }
}
