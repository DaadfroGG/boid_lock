#!/bin/bash
# Change the path to the directory where the program is located
make -C  /mnt/nfs/homes/dkermia/boid_lock/
ft_lock
until grep '"ft_lock":' <(xwininfo -tree -root) > /dev/null; do
	sleep 1
done
sleep 1
$(dirname $0)/a.out &
pid=$!
while pgrep -f "ft_lock +-d" > /dev/null; do
	sleep 1
done
kill $pid
