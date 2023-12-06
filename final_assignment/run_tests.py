"""This script tests all implementations of count_threes and records running 
times. Each implementation will be tested on sizes of 10000, 100000, 
1000000, 10000000, running 5 times each for an average. Results are saved to 
csv. Implementations are C programs and a Java Program in subfolders, each 
with a makefile.

Usage:
        $ make
        $ ./count_threes
"""

import subprocess
import csv
import re
import os

def parse_performance_metrics(line):
        match = re.match(r'"(.+)": ([\d.]+)', line)
        if match:
                metric, value = match.groups()
                return metric, float(value)
        return None

def run_program(command, folder='.'):
        env = os.environ.copy()
        command = command.split()
        if 'OMP_NUM_THREADS' in command[0]:
                omp_threads, num_threads = command[0].split('=')
                env[omp_threads] = num_threads
                command = command[1:]

        # Change directory to program's folder
        initial_dir = os.getcwd()
        os.chdir(f'{initial_dir}/{folder.lstrip("./")}')

        # Execute the command
        result = subprocess.run(
                command, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.STDOUT,
                text=True,
                env=env
        )
        
        execution_times = []
        for line in result.stdout.split('\n'):
                parsed_metric = parse_performance_metrics(line)
                if parsed_metric and parsed_metric[0] == 'executionTime':
                        execution_times.append(parsed_metric[1])

        # Return to initial directory
        os.chdir(initial_dir)
        return execution_times

def build_commands():
        array_sizes = [10000, 100000, 1000000, 10000000]
        thread_counts = [4, 8, 16, 32]
        commands = []

        # Build commands for each program
        for size in array_sizes:
                for count in thread_counts:
                        commands.append(
                                (f'OMP_NUM_THREADS={count} ./count_threes {size}', 
                                size, count, 'OpenMP', './c_openmp')
                        )
                        commands.append(
                                (f'java CountThrees {size} {count}', size, count, 
                                'Java', './java')
                        )
                        commands.append(
                                (f'/usr/lib64/mpi/gcc/openmpi4/bin/mpirun --hostfile ./remotes.txt -np {count} ./count_threes {size}', 
                                size, count, 'MPI', './mpi')
                        )
                        commands.append(
                                (f'./padded {size} {count}', size, count, 
                                'Padded', './padded')
                        )

                # CUDA program
                commands.append(
                        (f'./cuda_test {size}', size, 'N/A', 'CUDA', './cuda')
                )

        return commands

def export_to_csv(results, filename):
        with open(filename, 'w', newline='') as file:
                writer = csv.writer(file)
                writer.writerow(['Program', 'Array Size', 'Thread Count', 'Execution Time'])
                writer.writerows(results)

# Execute and record results
commands = build_commands()
results = []

for cmd, size, count, program, folder in commands:
        execution_times = run_program(cmd, folder=folder)
        for time in execution_times:
                results.append([program, size, count, time])

export_to_csv(results, 'performance_metrics.csv')
