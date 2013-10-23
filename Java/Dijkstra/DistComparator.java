/*
 * Comparator for comparing distance to the start vertex
 * between two vertice.
 */

import java.util.Comparator;

public class DistComparator implements Comparator<Vertex> {

    @Override
    public int compare(Vertex v1, Vertex v2) {
	if (v1.distToStart() < v2.distToStart()) {
	    return -1;
	} else if (v1.distToStart() > v2.distToStart()) {
	    return 1;
	} else {
	    return 0;
	}
    }
}