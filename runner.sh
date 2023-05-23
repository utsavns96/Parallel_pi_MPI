#!/bin/sh
#sleep commands prevent our jobs from running out of order and breaking the output
#case a => nodes=1:ppn=P
qsub -l nodes=1:ppn=1 -v case='a' hw2.pbs
qsub -l nodes=1:ppn=1 -v case='a' hw2_simple.pbs
sleep 5
qsub -l nodes=1:ppn=2 -v case='a'  hw2.pbs
qsub -l nodes=1:ppn=2 -v case='a' hw2_simple.pbs
sleep 5
qsub -l nodes=1:ppn=4 -v case='a' hw2.pbs
qsub -l nodes=1:ppn=4 -v case='a' hw2_simple.pbs
sleep 5
qsub -l nodes=1:ppn=8 -v case='a' hw2.pbs
qsub -l nodes=1:ppn=8 -v case='a' hw2_simple.pbs
sleep 5
qsub -l nodes=1:ppn=16 -v case='a' hw2.pbs
qsub -l nodes=1:ppn=16 -v case='a' hw2_simple.pbs
sleep 5
#case b => nodes=P:ppn=1
qsub -l nodes=2:ppn=1 -v case='b' hw2.pbs
qsub -l nodes=2:ppn=1 -v case='b' hw2_simple.pbs
sleep 5
qsub -l nodes=4:ppn=1 -v case='b' hw2.pbs
qsub -l nodes=4:ppn=1 -v case='b' hw2_simple.pbs
sleep 5
qsub -l nodes=8:ppn=1 -v case='b' hw2.pbs
qsub -l nodes=8:ppn=1 -v case='b' hw2_simple.pbs
sleep 5
#case c => nodes=P/2:ppn=2
qsub -l nodes=8:ppn=2 -v case='c' hw2.pbs
qsub -l nodes=8:ppn=2 -v case='c' hw2_simple.pbs