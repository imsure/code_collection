import java.util.ArrayList;

public class InsertionSort extends IntegerSort {

    public InsertionSort() {}
	
    public ArrayList<Integer> sort(ArrayList<Integer> array) {
	long start = System.currentTimeMillis();
	for(int j = 1; j < array.size(); j++)
	    {
		int key;
		key=array.get(j);
		int i = j - 1;
		
		while(i>=0 && array.get(i)>key) {
		    super.incNumOfComparison(1);
		    array.set(i+1, array.get(i));
		    i = i-1;
		}
		/* In each outer loop, there is at least one
		   comparasion it while loop not entered. */
		super.incNumOfComparison(1);
		array.set(i+1, key);
	    }
	long end = System.currentTimeMillis();
	long time = end - start;
	super.setExecTimeInMills(time);
	long secs = time / 1000;
	long mills = time - 1000 * secs;
	System.out.println("Sorting time: " + secs + " seconds and " + mills + " millisecs");
	System.out.println("Number of Comparison: " + super.getNumOfComparison());

	return array;
    }
}
