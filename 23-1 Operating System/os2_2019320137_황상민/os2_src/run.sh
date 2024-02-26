#! /bin/bash

cd /sys/fs/cgroup/cpuset
mkdir mycpu; cd mycpu
echo 0 > cpuset.cpus
echo 0 > cpuset.mems
echo $$ > tasks
cat tasks
cd /home/bbang3/os2

time_slice=$1
echo "Time slice: $time_slice"
echo $time_slice > /proc/sys/kernel/sched_rr_timeslice_ms
cat /proc/sys/kernel/sched_rr_timeslice_ms

filename_burst="burst_$time_slice.txt"
filename_stdout="stdout_$time_slice.txt"
rm $filename_burst
rm $filename_stdout
dmesg -c > /dev/null
for ((run=1; run <= $2; run++))
do
	echo "---------Run $run---------"
	echo "Running calculating process..."

	filename_tmp="tmp.txt"
	./cpu 2 30 > $filename_tmp
	echo "Done"

	cat $filename_tmp | grep "DONE" | grep "#00" >> $filename_stdout
	cat $filename_tmp | grep "DONE" | grep "#01" >> $filename_stdout
	cat $filename_tmp | grep "DONE"

	p1=$(cat $filename_tmp | grep "DONE" | grep "#00" | awk '{print $8}')
	p2=$(cat $filename_tmp | grep "DONE" | grep "#01" | awk '{print $8}')

	echo "P1: $p1"
	echo "P2: $p2"
	
	filename_dmesg="log_dmesg.txt"
	dmesg -k | grep CPUburst | grep -e "Pid: $p1" -e "Pid: $p2" > $filename_dmesg

	python3 parse.py $p1 $p2 >> $filename_burst
done
