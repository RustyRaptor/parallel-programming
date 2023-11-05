import matplotlib.pyplot as plt
 
 # Provided program output as a string
program_output = """
Counting 3s sequentially...
finished counting 3s
count: 99994272
Time taken: 1.528885 seconds
Counting 3s sequentially...
finished counting 3s
count: 100004192
Time taken: 1.521651 seconds
Counting 3s sequentially...
finished counting 3s
count: 100000476
Time taken: 1.523760 seconds
Counting 3s sequentially...
finished counting 3s
count: 100014445
Time taken: 1.524873 seconds
Counting 3s sequentially...
finished counting 3s
count: 100024069
Time taken: 1.527630 seconds
Counting 3s sequentially...
finished counting 3s
count: 100009123
Time taken: 0.762074 seconds
Counting 3s sequentially...
finished counting 3s
count: 100015500
Time taken: 0.763967 seconds
Counting 3s sequentially...
finished counting 3s
count: 100008976
Time taken: 0.763413 seconds
Counting 3s sequentially...
finished counting 3s
count: 100010319
Time taken: 0.764237 seconds
Counting 3s sequentially...
finished counting 3s
count: 99981263
Time taken: 0.762605 seconds
Counting 3s sequentially...
finished counting 3s
count: 100003901
Time taken: 0.442879 seconds
Counting 3s sequentially...
finished counting 3s
count: 100003640
Time taken: 0.382068 seconds
Counting 3s sequentially...
finished counting 3s
count: 100020536
Time taken: 0.381934 seconds
Counting 3s sequentially...
finished counting 3s
count: 100000475
Time taken: 0.385530 seconds
Counting 3s sequentially...
finished counting 3s
count: 100004132
Time taken: 0.381731 seconds
Counting 3s sequentially...
finished counting 3s
count: 99996709
Time taken: 0.254787 seconds
Counting 3s sequentially...
finished counting 3s
count: 100003625
Time taken: 0.191506 seconds
Counting 3s sequentially...
finished counting 3s
count: 100003638
Time taken: 0.254230 seconds
Counting 3s sequentially...
finished counting 3s
count: 100003219
Time taken: 0.191384 seconds
Counting 3s sequentially...
finished counting 3s
count: 100012445
Time taken: 0.191317 seconds
Counting 3s sequentially...
finished counting 3s
count: 99987909
Time taken: 0.133211 seconds
Counting 3s sequentially...
finished counting 3s
count: 100006337
Time taken: 0.096643 seconds
Counting 3s sequentially...
finished counting 3s
count: 99997158
Time taken: 0.096577 seconds
Counting 3s sequentially...
finished counting 3s
count: 100008637
Time taken: 0.125293 seconds
Counting 3s sequentially...
finished counting 3s
count: 99994796
Time taken: 0.131650 seconds
Counting 3s sequentially...
finished counting 3s
count: 99977435
Time taken: 0.075728 seconds
Counting 3s sequentially...
finished counting 3s
count: 100004478
Time taken: 0.074724 seconds
Counting 3s sequentially...
finished counting 3s
count: 100004689
Time taken: 0.076790 seconds
Counting 3s sequentially...
finished counting 3s
count: 100000961
Time taken: 0.070120 seconds
Counting 3s sequentially...
finished counting 3s
count: 99989321
Time taken: 0.096495 seconds
Counting 3s sequentially...
finished counting 3s
count: 99991306
Time taken: 0.072446 seconds
Counting 3s sequentially...
finished counting 3s
count: 100012069
Time taken: 0.081696 seconds
Counting 3s sequentially...
finished counting 3s
count: 99989876
Time taken: 0.079536 seconds
Counting 3s sequentially...
finished counting 3s
count: 99990048
Time taken: 0.091937 seconds
Counting 3s sequentially...
finished counting 3s
count: 99999415
Time taken: 0.065238 seconds
"""

# Split the output into lines
lines = program_output.strip().split("\n")

# Initialize lists to hold the times for each thread count
times_1_thread = []
times_2_threads = []
times_4_threads = []
times_8_threads = []
times_16_threads = []
times_32_threads = []
times_64_threads = []

# Initialize a list of lists to make it easier to append in a loop
all_times = [
        times_1_thread,
        times_2_threads,
        times_4_threads,
        times_8_threads,
        times_16_threads,
        times_32_threads,
        times_64_threads,
]

# Current thread count index
thread_count_index = 0

# Parse the output and fill the lists
for line in lines:
        if "Time taken:" in line:
                # Extract the time taken from the line
                time_taken = float(line.split()[-2])
                # Append the time to the appropriate list
                all_times[thread_count_index].append(time_taken)
                # Check if the current thread count's list is filled with 5 runs
                if len(all_times[thread_count_index]) == 5:
                        # Move to the next thread count
                        thread_count_index += 1

# Now all_times contains the lists of times for each thread count
print("Times for 1 thread:", times_1_thread)
print("Times for 2 threads:", times_2_threads)
print("Times for 4 threads:", times_4_threads)
print("Times for 8 threads:", times_8_threads)
print("Times for 16 threads:", times_16_threads)
print("Times for 32 threads:", times_32_threads)
print("Times for 64 threads:", times_64_threads)



# Assuming the previous code has been executed and all_times is populated

# Calculate the average time for each thread count
average_times = [sum(times) / len(times) for times in all_times]

# Calculate speedup compared to single-threaded execution
speedup = [average_times[0] / time for time in average_times]

# Thread counts
thread_counts = [1, 2, 4, 8, 16, 32]

# Plotting the speedup bar graph
plt.figure(figsize=(10, 6))
plt.bar(thread_counts, speedup, color='skyblue')

# Adding titles and labels
plt.title('Speedup by Thread Count')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup Factor')
plt.xticks(thread_counts)

# Display the bar graph
plt.show()