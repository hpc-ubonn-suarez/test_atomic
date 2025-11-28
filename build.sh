CFLAGS="-fopenmp -O3 -mtune=native -march=native"
LDFLAGS="-lm"

for exe in atomic_parallel_check atomic_test critical_parallel_check serial_check; do
  gcc ${CFLAGS} ${exe}.c -o ${exe} ${LDFLAGS}
done
