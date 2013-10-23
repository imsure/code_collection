/*
 * Implementation of merge sort.
 */

import java.util.ArrayList;

public class MergeSort extends IntegerSort {
	
    public MergeSort() {}
	
    public ArrayList<Integer> sort(ArrayList<Integer> array) {
	ArrayList<Integer> result;
		
	long start = System.currentTimeMillis();
	result = mergeSort(array);
	long end = System.currentTimeMillis();
		
	long exectime = end - start;
	super.setExecTimeInMills(exectime);
	long secs = exectime / 1000;
	long mills = exectime - 1000 * secs;
	System.out.println("Sorting time: " + secs + " seconds and " + mills + " millisecs");
	System.out.println("Number of Comparison: " + super.getNumOfComparison());
		
	return result; 
    }
	
    private ArrayList<Integer> mergeSort(ArrayList<Integer> data) {
	int len = data.size();
	if (len <= 1) {
	    return data;
	}
	//Split the array into two subarrays.
	ArrayList<Integer> left = new ArrayList<Integer>();
	ArrayList<Integer> right = new ArrayList<Integer>();
	int mid = len / 2;
		
	for (int i = 0; i < mid; i++) {
	    left.add(data.get(i));
	}
	for (int i = mid; i < len; i++) {
	    right.add(data.get(i));
	}
		
	return merge(mergeSort(left), mergeSort(right));
    }
	
    private ArrayList<Integer> merge(ArrayList<Integer> left, ArrayList<Integer> right) {
	ArrayList<Integer> result = new ArrayList<Integer>();
		
	while (left.size() > 0 || right.size() > 0) {
	    if (left.size() > 0 && right.size() > 0) {
		super.incNumOfComparison(1);
		if (left.get(0) < right.get(0)) {
		    result.add(left.get(0));
		    left.remove(0);
		} else {
		    result.add(right.get(0));
		    right.remove(0);
		}
	    } else if (left.size() > 0) {
		result.add(left.get(0));
		left.remove(0);
	    } else if (right.size() > 0) {
		result.add(right.get(0));
		right.remove(0);
	    }
	}
		
	return result;
    }
	
    public static void main(String[] args) {
	ArrayList<Integer> test = new ArrayList<Integer>();
	test.add(3);
	test.add(5);
	test.add(4);
	test.add(1);
	test.add(2);
	test.add(99);
	test.add(84);
	test.add(0);
	test.add(-1);
	test.add(-999);

	MergeSort ms = new MergeSort();
	test = ms.sort(test);

	System.out.println("Sort results:");
	for (int d : test) {
	    System.out.println(d);
	}
	System.out.print('\n');
    }
}
