#!/bin/python

import sys
import subprocess
import os


def set_threads(thread_count):
    """
    :param thread_count: the number of threads to use (set via OMP_NUM_THREADS)
    """
    # Set the OMP_NUM_THREADS environment variable
    os.environ['OMP_NUM_THREADS'] = str(thread_count)


def run_and_collect(execution_command):
    """
    Run the program with the specified thread count and return the execution time
    from the output of the program.

    :param program_name: Path to the program.
    :param args: List of arguments to pass to the program.
    
    :return: Execution time (float) from the program output.
    """
    # Run the program and capture the output
    result = subprocess.run(execution_command.split(" "), capture_output=True, text=True)
    
    output = result.stdout.strip()

    # Parse the execution time 
    try:
        # More sophisticated parsing logic may be required based on how the time is presented
        # Example: "Execution Time: 0.1234 seconds"
        # Below assumes program only reports exection time
        execution_time = float(output)
    except Exception as e:
        print(f"Error parsing output: {output}")
        return None

    return execution_time


class Execution:

    """
    :program: program name
    :args: arguments to program
    """
    def __init__(self, program, args):
        self.program=program
        self.args=args
        
    def execute_command(self):
        return f"{self.program} {self.args}"

    # PA1 TODO: implement the following 4 functions using taskset
    def execute_command_p_cores(self):
        return f"taskset 0x0000FFFF {self.execute_command()}"

    def execute_command_e_cores(self):
        return f"taskset 0xFFFF0000 {self.execute_command()}"

    # extra credit
    def execute_command_p_cores_no_smt(self):
        return f"taskset 0x00005555 {self.execute_command()}"

    def execute_command_4_p_cores_no_smt(self):
        return f"taskset 0x00000055 {self.execute_command()}"


def experiment(program_name, args, thread_max, n_trials):
    """
    Conducts an experiment running the program multiple times with each thread count
    in [0..thread_max] and returns the execution times averaged over repetitions.

    :param program_name: The program name or path to the executable.
    :param args: A list of arguments to pass to the program (excluding thread count).
    :param thread_max: The maximum number of threads to test.
    :param n_trials: number of time test is repeated

    :return: A list of execution times for each thread count.
    """
    times = []

    execution=Execution(program_name,args)

    for threads in range(1, thread_max + 1):
        # Run the program and record the time taken
        thread_times=[]
        for rep in range(n_trials):
           

            # PA1 TODO: use execute_command_* functions to collect elapsed_time data
            elapsed_time = run_and_collect(execution.execute_command())
                       
            if elapsed_time is not None:
                thread_times.append(elapsed_time)
                print(f"Threads: {threads}, Time: {elapsed_time:.4f} seconds")
            else:
                print(f"Failed to get execution time for {threads} threads.")
        
        times.append(time_aggregation_logic(thread_times))

    return times

def time_aggregation_logic(times):
    """
    logic for computing reported time from repeated exections
    currently computes average

    :param times: list of times to be aggregated

    :return: a single time representing average of list
    """
    return sum(times)/len(times)


if __name__ == "__main__":
    if len(sys.argv)<3:
        print('Usage: collect_data.py {program_name} "{arguments}" [max_threads] [n_trials]')
        exit(1)

    max_threads=6
    n_trials=5
    
    program_name = sys.argv[1]      # "path/to/your/program"  
    arguments = sys.argv[2]         # arguments, use "arg1 arg2" for multiple
    if (len(sys.argv)>3):
        max_threads = int(sys.argv[3])  # maximum number of threads
    if (len(sys.argv)>4):
        n_trials = int(sys.argv[4]) # number of trials
        
    results = experiment(program_name, arguments, max_threads, n_trials)
    
    print("Execution time for each thread count:")
    for t in range(0,max_threads):
        print(f"{t+1}, {results[t]}")
