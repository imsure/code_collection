/*
 * Implementation of in-place heap sort using a
 * Max-Heap which is a specific data structure. 
 *
 * Note: The heap is not implemented as a template,
 * now it can only store integers.
 *
 * TODO: Implement a generic heap can be applied on any
 * data types.
 */

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class HeapSort extends IntegerSort {

    private ArrayList<Integer> heap; // An arraylist represents a heap.
    private int heapSize;
    private boolean DEBUG = false;

    public HeapSort() {
    }

    /* Given the index 'i' of a node,
       Return the index of its parent. */
    private int parent(int i) {
	if (i > 0) {
	    return (i - 1) / 2;
	} else {
	    return 0;
	}
    }

    /* Given the index 'i' of a node,
       Return the index of its left child. */
    private int left(int i) {
	return i*2 + 1;
    }

    /* Given the index 'i' of a node,
       Return the index of its left child. */
    private int right(int i) {
	return i*2 + 2;
    }
    
    /* Given the index 'i' of a node,
       Return whether the node is a left. */
    private boolean isLeaf(int i) {
	return i >= (this.heapSize / 2);
    }

    /* Maintain the max heap property for the subtree
       rooted at index 'i'. The precondition is that
       subtrees rooted at left(i) and right(i) are max heaps. */
    private void maxHeapify(int i) {
	/* Check the base case, if it is a leaf node,
	   it aleary obeys the max heap property. */
	if (isLeaf(i)) {
	    return;
	}
	int index_l = left(i);
	int index_r = right(i);
	int largest = i; // index of the largest node.

	// FIXME: this is not correct!
	super.incNumOfComparison(2);
	if ((index_l < this.heapSize) &&
	    (this.heap.get(index_l) > this.heap.get(largest))) {
	    largest = index_l;
	}
	if ((index_r < this.heapSize) &&
	    this.heap.get(index_r) > this.heap.get(largest)) {
	    largest = index_r;
	}
	if (largest != i) {
	    /* After swapping, node 'i' satisifies max-heap property. */
	    Collections.swap(this.heap, largest, i);
	    /* But subtree rooted at 'largest' might violate max-heap
	       property, so apply the procedure recursively on this
	       subtree until reach a leaf node. */
	    maxHeapify(largest);
	}
    }

    /*
     * Build a max heap buttom up.
     */
    private void buildMaxHeap() {
	/* For a heap, we've alreay proved that the elements
	   in the subarray A[n/2, n-1] are all leaves, so each
	   is one element heap to begin with. To build a max heap,
	   just go through the remaining nodes buttom up, that is,
	   from index (n/2-1) to index 0. */
	for (int i = (this.heapSize/2 - 1); i >= 0; i--) {
	    maxHeapify(i);
	}

	if (DEBUG) {
	    System.out.println("Max Heap:");
	    for (int e : this.heap) {
		System.out.println(e);
	    }
	}

    }

    /*
     * Sort routine.
     * First, build a max heap.
     * Then, 
     */
    public ArrayList<Integer> sort(ArrayList<Integer> array) {
	this.heap = array;
	this.heapSize = array.size();

	long start = System.currentTimeMillis();
	
	/* Must call 'buildMaxHeap()' first!!! */
	buildMaxHeap();

	/* index used to keep track of the last position of heap. */
	int last = this.heapSize - 1; 
	while (this.heapSize >= 1) {
	    Collections.swap(this.heap, 0, last);
	    last--;
	    this.heapSize--;
	    maxHeapify(0);
	}

	long end = System.currentTimeMillis();

	long exectime = end - start;
	super.setExecTimeInMills(exectime);

	long secs = exectime / 1000;
	long mills = exectime - 1000 * secs;
	System.out.println("Sorting time: " + secs + " seconds and " + mills + " millisecs");
	System.out.println("Number of Comparison: " + super.getNumOfComparison());
	
	return this.heap;
    }

    public static void main(String[] args) {
	ArrayList<Integer> test = new ArrayList<Integer>();
	Random generator = new Random();

	for (int i = 0; i < 1000000; i++) {
	    test.add(generator.nextInt());
	}

	HeapSort hs = new HeapSort();

	test = hs.sort(test);
	System.out.println("Sort results:");
	for (int d : test) {
	    System.out.println(d);
	}
	System.out.print('\n');
    }

}


