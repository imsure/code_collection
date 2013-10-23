/*
 * This is an abstract class for a specific sorting algorithm
 * to subclass.
 *
 * It defines some generic attributes and methods.
 */

import java.util.ArrayList;

public abstract class IntegerSort {
    private long numOfComparison = 0;
    private long execTime = 0;

    public long getNumOfComparison() {
	return numOfComparison;
    }

    public long getExecTimeInMills() {
	return execTime;
    }

    protected void incNumOfComparison(long inc) {
	numOfComparison += inc;
    }

    protected void setExecTimeInMills(long time) {
	execTime = time;
    }

    abstract ArrayList<Integer> sort(ArrayList<Integer> array);
}

