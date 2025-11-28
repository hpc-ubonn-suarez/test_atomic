echo "running atomic_test using 10 threads"
OMP_NUM_THREADS=10 ./atomic_test
echo

echo "running atomic_parallel_check using 10 threads, check your CPU usage!"
sleep 2
time OMP_NUM_THREADS=10 ./atomic_parallel_check
echo

echo "running critical_parallel_check using 10 threads, check your CPU usage!"
sleep 2
time OMP_NUM_THREADS=10 ./critical_parallel_check
echo

echo "running serial_check"
sleep 2
time ./serial_check
echo


