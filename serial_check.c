#include <stdio.h>
#include <math.h>

double f(int tid)
{
  const size_t max_iter = 25000000;
  double ret = 2;
  for(size_t n = 0; n < max_iter; ++n){
    ret += tid*sqrt(ret)*log(ret)/max_iter;
  }
  return ret;
}

int main(void)
{
  double sum_ftid = 0.0;

  for(size_t i = 0; i < 100; ++i){
    sum_ftid += f(i/10);
  }

  printf("sum_ftid: %f\n", sum_ftid);
}

