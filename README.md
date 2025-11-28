Subsequent to the discussion on 2025-11-28 I implemented the four little test programs attached here.

Compile with `./build.sh` (you need a gcc installed).

Run with `./run.sh` 

# atomic_test

This program test the claim in the lecture that 

```
#pragma omp atomic
x += f(x);
```

leads to incorrect results while 

```
#pragma omp critical
{
  x += f(x);
}
```

is correct, **unless** f(x) has very special features and the order of execution matters.

This has been achieved by using the following `f(x)`:

```
double f(double x, int tid)    
{    
  if(x > 1000){    
    return 2*x*tid;    
  } else {    
    return x*tid;    
  }    
}    
```

which is passed the current value of `x` and the thread id and the return value depends on the current value of `x` and on the order of execution because it also depends on `tid`.

Further, we test that

```
#pragma omp atomic
  sum_ftid += f(omp_get_thread_num(), omp_get_thread_num());
```

produces the correct result because there is no data race between the accumulation variable and the function arguments.

Finally, we check serially -- by simulating `omp_get_thread_num()` -- what the correct value for `sum_fx` actually should be (when using 10 threads and executing the loop chunks sequentially).

Run `./atomic_test` multiple times to see how `sum_fx_crit` and `sum_fx` change!

# atomic_parallel_check

Here we test whether the statement on the right-hand side of

```
#pragma omp atomic
  sum_ftid += f(omp_get_thread_num());
```

gets executed in parallel by implementing a "very expensive" f(x) 

Using `htop` we can confirm that indeed 10 threads are busy.

The parallelised loop over 100 iterations uses `schedule(static,10)` to ensure that the first thread gets the first 10 iterations, the second thread gets the next 10 and so on.

# critical_parallel_check

We run exactly the same operations as in `atomic_parallel_check` but we have

```
#pragma omp critical
{
  sum_ftid += f(omp_get_thread_num());
}
```

Using `htop` we can see that only one thread at a time runs the expensive function.

The parallelised loop over 100 iterations uses `schedule(static,10)` to ensure that the first thread gets the first 10 iterations, the second thread gets the next 10 and so on.

# serial_check

We run exactly the same operations as in `critical_parallel_check` but without any OpenMP and see that it takes just as long as `critical_parallel_check`.

# sample output

```
 $ ./run.sh 
running atomic_test using 10 threads, three times
run 1
sum_fx_serial: 5.4219740441e+84   sum_fx_crit: 3.9355276438e+85   sum_fx: 5.7049933651e+83
sum_ftid: 2.8500000000e+03
run 2
sum_fx_serial: 5.4219740441e+84   sum_fx_crit: 4.6631903071e+85   sum_fx: 2.6529917062e+82
sum_ftid: 2.8500000000e+03
run 3
sum_fx_serial: 5.4219740441e+84   sum_fx_crit: 4.7180804504e+85   sum_fx: 5.4710584520e+80
sum_ftid: 2.8500000000e+03

running atomic_parallel_check using 10 threads, check your CPU usage!
sum_ftid: 8323.533309

real	0m4.524s
user	0m44.809s
sys	0m0.077s

running critical_parallel_check using 10 threads, check your CPU usage!
sum_ftid: 8323.533309

real	0m39.773s
user	0m40.341s
sys	0m0.001s

running serial_check
sum_ftid: 8323.533309

real	0m39.614s
user	0m39.611s
sys	0m0.001s
```
