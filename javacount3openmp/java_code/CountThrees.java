import java.util.*;

public class CountThrees implements Runnable {
        private static final int ARRAY_LENGTH = 1000000;
        private static final int MAX_THREADS = 10;
        private static final int MAX_RANGE = 100;
        private static final Random random = new Random();
        private static int count = 0;
        private static final Object lock = new Object();
        private static int[] array;
        private static Thread[] threads;

        public static void main(String[] args) {
                array = new int[ARRAY_LENGTH];

                // Initialize the elements in the array
                for (int i = 0; i < array.length; i++) {
                        array[i] = random.nextInt(MAX_RANGE);
                }
                long startTime = System.nanoTime();

                // Create the threads
                CountThrees[] counters = new CountThrees[MAX_THREADS];
                int lengthPerThread = ARRAY_LENGTH / MAX_THREADS;
                for (int i = 0; i < counters.length; i++) {
                        counters[i] = new CountThrees(
                                i * lengthPerThread, 
                                lengthPerThread
                        );
                }

                // Run the threads
                threads = new Thread[MAX_THREADS];
                for (int i = 0; i < counters.length; i++) {
                        threads[i] = new Thread(counters[i]);
                        threads[i].start();
                }

                for (int i = 0; i < counters.length; i++) {
                        try {
                                threads[i].join();
                        } catch (InterruptedException e) {
                                // Do nothing
                        }
                }
                long endTime = System.nanoTime();
                // Print the number of threes
                System.out.println("Number of threes: " + count);

                // compute the elapsed time in nanoseconds
                long duration = (endTime - startTime);  

                // convert to seconds
                double seconds = (double)duration / 1_000_000_000.0; 

                System.out.println("Time taken: " + seconds + " seconds");
        }

        private int startIndex;
        private int elements;
        private int myCount = 0;

        public CountThrees(int start, int elem) {
                startIndex = start;
                elements = elem;
        }

        // Overload of run method in the Thread class
        public void run() {
                // Count the number of threes
                for (int i = 0; i < elements; i++) {
                        if (array[startIndex + i] == 3) {
                                myCount++;
                        }
                }

                synchronized (lock) {
                        count += myCount;
                }
        }
}