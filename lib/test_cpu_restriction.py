#!/bin/python

import collect_data as CD
import os, subprocess, sys, time, signal

def run_and_detach(cmd):
    p = subprocess.Popen(
        cmd.split(" "),
        stdin=subprocess.DEVNULL,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        close_fds=True,
        start_new_session=True
    )
    return p.pid

def get_affinity(pid):
    return os.sched_getaffinity(pid)


if __name__=="__main__":
    ex = CD.Execution("yes","")

    comm = ex.execute_command()
    # replace with: (e.g.)
    #comm = ex.execute_command_e_cores()

    pid = run_and_detach(comm)
    time.sleep(0.05)
    affinity = get_affinity(pid)
    print(affinity)

    os.kill(pid, signal.SIGTERM)
