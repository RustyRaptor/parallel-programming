import java.util.*;

public class CountThrees implements Runnable {
        private static int ARRAY_LENGTH;
        private static int MAX_THREADS;
        private static final int MAX_RANGE = 100;
        private static final Random random = new Random();
        private static int count = 0;
        private static final Object lock = new Object();
        private static int[] array;
        private static Thread[] threads;

        public static void main(String[] args) {
                if (args.length != 2) {
                        System.err.println("Usage: java CountThrees <array_length> <max_threads>");
                        System.exit(1);
                }
                try {
                    ARRAY_LENGTH = Integer.parseInt(args[0]);
                    MAX_THREADS = Integer.parseInt(args[1]);
                } catch (NumberFormatException e) {
                    System.err.println("Invalid argument: " + args[0] + " or " + args[1] + " is not a valid integer");
                    System.exit(1);
                }
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
                // System.out.println("Number of threes: " + count);

                // compute the elapsed time in nanoseconds
                long duration = (endTime - startTime);  


                printPerformanceMetrics("executionTime", duration);
        }

        private int startIndex;
        private int elements;
        private int myCount = 0;

        public CountThrees(int start, int elem) {
                startIndex = start;
                elements = elem;
        }

        
        public static void printPerformanceMetrics(
                String metric,
                double value
                )
        {
                System.out.printf("\"%s\": %f%n", metric, value);
        }
            

        // Overload of run method in the Thread class
        public void run() {
                // Count the number of threes
                for (int i = 0; i < elements; i++) {
                        if (array[startIndex + i] == 3) {
                                myCount++;
                        }
                }

                // synchronized (lock) {
                //         count += myCount;
                // }

                count += myCount;
        }
}
