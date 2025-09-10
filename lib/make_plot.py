#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def create_speedup_plot(csv_files, plot_title="Speedup Plot"):
    """
    Generates a speedup plot from one or more CSV files.
    """
    plt.figure(figsize=(10, 6))

    for file_path in csv_files:
        # get data into df
        run_data = pd.read_csv(file_path)

        # Check for required columns
        if 'thread_count' not in run_data.columns or 'execution_time' not in run_data.columns:
            print(f"Error: CSV file {file_path} is missing required columns ('thread_count' or 'execution_time'). Skipping.")
            continue

        # Group by thread count and calculate the average execution time
        avg_times = run_data.groupby('thread_count')['execution_time'].mean().reset_index()
        avg_times.sort_values('thread_count', inplace=True)

        # Calculate speedup
        avg_times['speedup'] = avg_times.iloc[0]['execution_time'] / avg_times['execution_time']

        # get the run name
        label = file_path.split("/")[-1].split(".")[0]

        plt.plot(avg_times['thread_count'], avg_times['speedup'], marker='o', label=label)

    # Plot an ideal linear speedup line
    max_threads = run_data['thread_count'].max()
    plt.plot(range(1, max_threads + 1), range(1, max_threads + 1), 'r--', label='Ideal Speedup')

    # Add plot elements
    plt.title(plot_title.split('/')[-1])
    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'{plot_title}.png')

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: plot_data.py <plot_name> <file1.csv> [file2.csv] ...")
        sys.exit(1)

    create_speedup_plot(sys.argv[2:], sys.argv[1])