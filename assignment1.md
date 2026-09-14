# ECE 759 - Assignment 1

GitHub link: https://github.com/sssxrf/repo759/HW01

## 1. Required reading

I went through a) through c) and understand how to time code, how to submit my assignments with git, and what the recommended workflow is when it comes to working on my assignment.

## 2. Bash commands

a) `cd somedir`

b) `cat sometext.txt`

c) `tail -n 5 sometext.txt`

d) `tail -n 5 *.txt`

e) `for i in {0..6}; do echo "$i"; done`

## 3. Euler modules

a) No modules are loaded when I log in on Euler.

b) The GCC version available without loading any modules is 14.3.1.

c) The CUDA-related modules returned by `module avail cuda` are:

- `nvidia/cuda/10.2.2`
- `nvidia/cuda/11.0.3`
- `nvidia/cuda/11.3.1`
- `nvidia/cuda/11.6.0`
- `nvidia/cuda/11.8.0`
- `nvidia/cuda/12.0.0`
- `nvidia/cuda/12.1.0`
- `nvidia/cuda/12.2.0`
- `nvidia/cuda/12.5.0`
- `nvidia/cuda/12.9.1`
- `nvidia/cuda/13.0.0`
- `nvidia/nvhpc-hpcx-cuda11/24.5`
- `nvidia/nvhpc-hpcx-cuda12/23.11`
- `nvidia/nvhpc-hpcx-cuda12/24.5`

d) Valgrind (`valgrind/3.26.0`) is available as a module on Euler. Valgrind is a programming tool used to detect memory-management problems such as invalid memory accesses and memory leaks.

## 5. Slurm tools

a) By default, a Slurm job on Euler begins execution in the directory from which `sbatch` was invoked, unless another working directory is explicitly specified.

b) `SLURM_JOB_ID` is an environment variable containing the unique job ID assigned by Slurm to the running job.

c) I can track my pending and running jobs using `squeue -u "$USER"`.

d) I can cancel a queued or running job using `scancel JOB_ID`.

e) `#SBATCH --gres=gpu:1` requests one GPU as a generic resource for each node allocated to the job.

f) `#SBATCH --array=0-9` creates a job array with ten tasks indexed from 0 through 9.
