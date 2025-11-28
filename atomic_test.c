#include <stdio.h>
#include <omp.h>

double f(double x, int tid)
{
  if(x > 1000){
    return 2*x*tid;
  } else {
    return x*tid;
  }
}

int main(void)
{
  const size_t max_iter = 100;

  double sum_fx_serial = 1.e-1;
  double sum_fx = 1.e-1;
  double sum_fx_crit = 1.e-1;
  double sum_ftid = 0.0;
  
  for(size_t i = 0; i < max_iter; ++i){
    sum_fx_serial += f(sum_fx_serial, i/10);
  }

#pragma omp parallel for schedule(static,10)
  for(size_t i = 0; i < max_iter; ++i){
    // this is wrong because there is a double race on sum_fx
    // twice on the left (because sum_fx is read, accumulated and then written), which is
    // taken care of by the atomic statement
    // However, there is also a race on the right, because the "current" value (whatever that means) of sum_fx
    // must be copied into the function argument of f(x)
#pragma omp atomic
    sum_fx += f(sum_fx, omp_get_thread_num());

    // one would think that this works, but f(x) has been constructed such that
    // the value returned by it for a given thread depends on the ordering in which the
    // threads call it
#pragma omp critical
    {
      sum_fx_crit += f(sum_fx_crit, omp_get_thread_num());
    }

    // here there is no data race on the right and atomic access is only required
    // for the accumulation of sum_ftid
#pragma omp atomic
    sum_ftid += f(omp_get_thread_num(), omp_get_thread_num());
  }

  printf("sum_fx_serial: %.10e   sum_fx_crit: %.10e   sum_fx: %.10e\n", sum_fx_serial, sum_fx_crit, sum_fx);
  printf("sum_ftid: %.10e\n", sum_ftid);
}

