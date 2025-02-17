#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

static constexpr double f(double a) 
{
  double c = 0.8;
  return sin(c * a) * cos(c * a);
}

static constexpr double fi(double a) 
{
  double c = 0.8;
  return (1 / (2 * c)) * pow(sin(c * a), 2);
}

int main(int argc, char *argv[]) 
{
  int done = 0, n, myid, numprocs, i;
  double myfunk, funk, h, sum, x;
  double xl = 0.5, // low border
      xh = 2.0;     // high border
  double startwtime, endwtime;
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status stats;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Get_processor_name(processor_name, &namelen);
  fprintf(stderr, "Process %d on %s\n", myid, processor_name);
  fflush(stderr);
  n = 10000000;
  while (!done) 
  {
    if (myid == 0) 
    {
      std::cout << "Using interval count: " << n << std::endl;
      startwtime = MPI_Wtime();

      for (i = 1; i < numprocs; i++) 
      {
        MPI_Send(&n,              /* buffer */
                 1,               /* one data */
                 MPI_INT,         /* type */
                 i,               /* to which node */
                 1,               /* tag of message */
                 MPI_COMM_WORLD); /* common communicator */
      }
    } 
    else 
    {
      MPI_Recv(&n,             /* buffer */
               1,              /* one data */
               MPI_INT,        /* type */
               0,              /* from which node */
               1,              /* tag of message */
               MPI_COMM_WORLD, /* common communicator */
               &stats);        /* status of errors */
    }
    if (n == 0)
      done = 1;
    else 
    {
      h = (xh - xl) / (double)n;//вычисляем шаг
      //локальная сумма
      sum = 0.0;
      for (i = myid + 1; i <= n; i += numprocs) 
      {
        x = xl + h * ((double)i - 0.5);
        sum += f(x);
      }
      //локальный интеграл
      myfunk = h * sum;
      printf("Process %d SUMM %.16f\n", myid, myfunk);
      // Отправка локальной суммы в узел 0
      if (myid != 0) 
      {
        MPI_Send(&myfunk,         /* buffer */
                 1,               /* one data */
                 MPI_DOUBLE,      /* type */
                 0,               /* to which node */
                 1,               /* tag of message */
                 MPI_COMM_WORLD); /* common communicator */
      }
      if (myid == 0) 
      {
        funk = myfunk;
        for (i = 1; i < numprocs; i++) {
          MPI_Recv(&myfunk, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &stats);
          funk += myfunk;
        }
        printf("Integral is approximately %.16f, Error %.16f\n", funk,
               funk - fi(xh) + fi(xl));
        endwtime = MPI_Wtime();
        printf("Time of calculation = %f\n", endwtime - startwtime);
      }
      done =1;
    }
  }
  MPI_Finalize();
}
