echo "running atomic_test using 10 threads, three times"
for i in 1 2 3; do
  echo run $i
  OMP_NUM_THREADS=10 OMP_PROC_BIND=spread OMP_PLACES=threads ./atomic_test
done
echo

echo "running atomic_parallel_check using 10 threads, check your CPU usage!"
sleep 2
time OMP_NUM_THREADS=10 OMP_PROC_BIND=spread OMP_PLACES=threads ./atomic_parallel_check
echo

echo "running critical_parallel_check using 10 threads, check your CPU usage!"
sleep 2
time OMP_NUM_THREADS=10 OMP_PROC_BIND=spread OMP_PLACES=threads ./critical_parallel_check
echo

echo "running serial_check"
sleep 2
time numactl -C 0 ./serial_check
echo


