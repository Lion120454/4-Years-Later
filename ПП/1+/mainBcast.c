#include <mpi.h>
#include <stdio.h>
#include <math.h>

static double f(double a);
static double fi(double a);

int main(int argc, char *argv[])
{
    int done = 0, n, myid, numprocs, i;
    double myfunk, funk, h, sum, x;
    double xl = 0.5, xh = 2;
    double startwtime, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status stats;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    n = 0;
    while (!done)
    {
        if (myid == 0)
        {
            printf("Enter the number of intervals (0 quit) ");

            fflush(stdout);
            scanf("%d", &n);

            startwtime = MPI_Wtime();
        }

            MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            done = 1;
        else
        {
            h = (xh - xl) / (double)n;
            sum = 0.0;
            for (i = myid + 1; i <= n; i += numprocs)
            {
                x = xl + h * ((double)i - 0.5);
                sum += f(x);
            }
            myfunk = h * sum;

            MPI_Reduce(&myfunk, &funk, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

            if (myid == 0)
            {
                endwtime = MPI_Wtime();

                printf("Integral is approximately  %.16f, Error   %.16f\n", funk, abs(funk - fi(xh) + fi(xl)));
                printf("Time of calculation = %f\n", endwtime - startwtime);
            }
            done=1;
        }
    }
    MPI_Finalize();
}

static double f(double a)
{   
    double c = 0.8;
    return sin(c * a) * cos(c * a);
}

static double fi(double a)
{
    double c = 0.8;
    return (1 / (2 * c)) * pow(sin(c * a), 2);
}