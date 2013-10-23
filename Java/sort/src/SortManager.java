/*
 * File: SortManager.java
 * 
 * Description:
 * Take parameters from command line and control the sorting process.
 */

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.File;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;
import java.util.HashMap;
import java.util.Formatter;
import java.util.Arrays;

public class SortManager {
    
    private String inputFile;
    private Set<Character> validTypes;
    // A map mapping sort type to its stats.
    private Map<Character, long[]> sortStats;
    // A map mapping sort type to its full name.
    private Map<Character, String> sortType;
    private ArrayList<Integer> data;
    private String types; // used to store sort types passed from command line.
    private final String outputDir = "output";

    private final int NUM_TYPE = 4; // we currently support 4 type of sorts.
	
    /*
     * Constructor function used to initialize an object.
     */
    public SortManager(String [] args) {
	data = new ArrayList<Integer>();

	sortType = new HashMap<Character, String>();
	sortType.put('i', "Insertion Sort");
	sortType.put('m', "Merge Sort");
	sortType.put('h', "Heap Sort");
	sortType.put('q', "Quick Sort");

	validTypes = sortType.keySet();

	sortStats = new HashMap<Character, long[]>();
	for (char t : validTypes) {
	    long[] a = {0, 0};
	    sortStats.put(t, a);
	}

	File dir = new File(outputDir);
	if (!dir.exists()) {
	    System.out.println("creating directory: " + outputDir);
	    dir.mkdir();
	}

	parseArgs(args);
    }

    private void updateStats(char type, long numcmp, long mills) {
	long[] stats = sortStats.get(type);
	stats[0] = numcmp;
	stats[1] = mills;
    }

    private void startSorting(char type) {

	/* Make a deep copy of the unsorted data. */
	ArrayList<Integer> array = new ArrayList<Integer>(data);

	if (type == 'i') {
	    System.out.println("Using insertion sort.");
	    InsertionSort is = new InsertionSort();
	    array = is.sort(array);
	    
	    updateStats(type, is.getNumOfComparison(), is.getExecTimeInMills());
	    
	} else if (type == 'q') {
	    System.out.println("Using quick sort.");
	    QuickSort qs = new QuickSort();
	    array = qs.sort(array);
	    
	    updateStats(type, qs.getNumOfComparison(), qs.getExecTimeInMills());
	    
	} else if (type == 'm') {
	    System.out.println("Using merge sort.");
	    MergeSort ms = new MergeSort();
	    array = ms.sort(array);

	    updateStats(type, ms.getNumOfComparison(), ms.getExecTimeInMills());
	    
	} else if (type == 'h') {
	    System.out.println("Using heap sort.");
	    HeapSort hs = new HeapSort();
	    array = hs.sort(array);

	    updateStats(type, hs.getNumOfComparison(), hs.getExecTimeInMills());
       	}
	System.out.println();

	writeOutput(type, array);
    }

    private void writeOutput(char type, ArrayList<Integer> array) {
	String prefix = ""+type;
	String sortedFile = outputDir+'/'+prefix+'_'+inputFile+".sorted";
	String statsFile = outputDir+'/'+prefix+'_'+inputFile+".stats";

	try {
	    FileWriter fstream = new FileWriter(sortedFile);
	    BufferedWriter out = new BufferedWriter(fstream);

	    for (int d : array) {
		out.write(Integer.toString(d)+'\n');
	    }
	    out.close();

	    fstream = new FileWriter(statsFile);
	    out = new BufferedWriter(fstream);

	    long[] stats = sortStats.get(type);
	    out.write(Long.toString(stats[0])+'\n');
	    out.write(Long.toString(stats[1])+" milliseconds\n");

	    out.close();

	} catch (Exception e) {
	    System.err.println("Error: " + e.getMessage());
	}
     }

    private void writeComparisonReport() {

	/* If only one type applied, no need to generate
	   comparison report. */
	if (types.length() == 1) {
	    return;
	}

	String combinedReport = outputDir+'/'+types+'_'+inputFile+".combined";

	try {
	    FileWriter fstream = new FileWriter(combinedReport);
	    BufferedWriter out = new BufferedWriter(fstream);

	    StringBuilder sb = new StringBuilder();
	    Formatter fmt = new Formatter(sb);

	    fmt.format("%15s | %25s | %30s | %10s\n",
		       "Algorithm", "Number of Comparison",
		       "Execution Time (Millseconds)", "Speed Rank");

	    char[] hyphens = new char[90];
	    Arrays.fill(hyphens, '-');
	    fmt.format(new String(hyphens)+'\n');

	    /* TODO: This is pretty ugly, need to rewrite the code later! */
	    int rank = 1;
	    while (types.length() > 0) {
		int index = 0;
		long shortest = sortStats.get(types.charAt(0))[1];
		for (int i = 1; i < types.length(); i++) {
		    long[] stats = sortStats.get(types.charAt(i));
		    if (stats[1] < shortest) {
			index++;
			shortest = stats[1];
		    }
		}
		//System.out.println(shortest);
		fmt.format("%15s | %25s | %30s | %10s\n",
			   sortType.get(types.charAt(index)),
			   sortStats.get(types.charAt(index))[0],
			   sortStats.get(types.charAt(index))[1],
			   rank);
		rank++;
		types = types.substring(0, index) + types.substring(index+1);
		//System.out.println(types);
	    }

	    // for (int i = 0; i < types.length(); i++) {
	    // 	long[] stats = sortStats.get(types.charAt(i));

	    // 	fmt.format("%15s | %25s | %30s\n",
	    // 		   sortType.get(types.charAt(i)), stats[0], stats[1]);
	    // }
	    out.write(sb.toString());

	    out.close();
	
	} catch (Exception e) {
	    System.err.println("Error: " + e.getMessage());
	}
    }

    public void start() {
	for (int i = 0; i < types.length(); i++) {
	    startSorting(types.charAt(i));
	}

	writeComparisonReport();
    }

    /*
     * Read data from file.
     */
    private void readData() {
	try {
	    FileInputStream fin = new FileInputStream(inputFile);
	    DataInputStream din = new DataInputStream(fin);
	    BufferedReader br = new BufferedReader(new InputStreamReader(din));
			
	    String line;
	    while ((line = br.readLine()) != null) {
		data.add(Integer.parseInt(line));
	    }
	    System.out.println("Size of data: " + data.size());
	    System.out.println();
	} catch (Exception e) {
	    System.err.println("IO Error: " + e.getMessage());
	}
    }

    private void printHelp() {
	System.out.println("\nUsage:");
	System.out.println("java -jar MySort.jar <sort type(s)> <input file name>\n");
	System.out.println("Supported sort types:\n");

	StringBuilder sb = new StringBuilder();
	Formatter fmt = new Formatter(sb);
	fmt.format("%10s | %20s\n", "sort type", "full name");
	char[] hyphens = new char[35];
	Arrays.fill(hyphens, '-');
	fmt.format(new String(hyphens)+'\n');

	for (char t : validTypes) {
	    fmt.format("%10s | %20s\n", Character.toString(t), sortType.get(t));
	}
	System.out.print(sb.toString());

	System.out.println("\nHint:");
	System.out.println("You can use any combination of the above types.");
	System.out.println("For example, if you want to use all of them at once,");
	System.out.println("just pass the first argument as hmqi.\n");

	System.exit(-1);
    }

    private void quit() {
	System.out.println("\nYou can use '-h' to find help.\n");
	System.exit(-1);
    }

    /*
     * Used to parse the command line arguments.
     */
    private void parseArgs(String [] args) {

	if (args.length == 2) {
	    
	    /* Take the first argument which is the type of sort.*/
	    if (args[0].length() <= NUM_TYPE) {
		types = args[0];
		for (int i = 0; i < types.length(); i++) {
		    if (!validTypes.contains(types.charAt(i))) {
			System.err.println("Sort type "+types.charAt(i)+" is not supported.");
			System.exit(-1);
		    }
		}
	    } else {
		System.err.println("We currently only support four types of sort.");
		quit();
	    }
			
	    /* Take the second argument which is the name of file.*/
	    inputFile = args[1];
			
	} else {
	    if (args.length == 1 && args[0].equals("-h")) {
		printHelp();
	    } else {
		System.err.println("Please provide two arguments!");
		quit();
	    }
	}
    }
	
    /*
     * Main function is the entry point of the program.
     */
    public static void main(String [] args) {
	SortManager sort_manager = new SortManager(args);
	sort_manager.readData();
	sort_manager.start();
    }
}
