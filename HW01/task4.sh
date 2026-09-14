#!/bin/bash
#SBATCH -p research
#SBATCH -c 2
#SBATCH -J FirstSlurm
#SBATCH -o FirstSlurm.out
#SBATCH -e FirstSlurm.err

hostname
