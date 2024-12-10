#include <mpi.h>
#include <stdio.h>
#include <math.h>

static double f(double a, double p);
static double fi(double a, double p);

int main(int argc, char *argv[])
{
    int done = 0, n, myid, numprocs, i;
    double myfunk, funk, h, sum, x;
    double xl, xh, p;
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
        int sizePack = 28;
        char packBuffer[sizePack];
        int packPosition = 0;
        if (myid == 0)
        {
            printf("Enter the number of intervals (0 quit) ");
            fflush(stdout);
            scanf("%d", &n);
            MPI_Pack(&n, 1, MPI_INT, &packBuffer, sizePack, &packPosition, MPI_COMM_WORLD);
            if (n != 0)
            {            
                printf("Enter xl ");
                fflush(stdout);
                scanf("%lf", &xl);

                printf("Enter xh ");
                fflush(stdout);
                scanf("%lf", &xh);

                printf("Enter the parameter ");
                fflush(stdout);
                scanf("%lf", &p);

                MPI_Pack(&xl, 1, MPI_DOUBLE, &packBuffer, sizePack, &packPosition, MPI_COMM_WORLD);
                MPI_Pack(&xh, 1, MPI_DOUBLE, &packBuffer, sizePack, &packPosition, MPI_COMM_WORLD);
                MPI_Pack(&p, 1, MPI_DOUBLE, &packBuffer, sizePack, &packPosition, MPI_COMM_WORLD);

                startwtime = MPI_Wtime();
            }
        }

        MPI_Bcast(&packBuffer, sizePack, MPI_PACKED, 0, MPI_COMM_WORLD);
        if (myid == 0)
        {
            if (n == 0)
            {
                done = 1;
                continue;
            }
        }
        else
        {
            MPI_Unpack(&packBuffer, sizePack, &packPosition, &n, 1, MPI_INT, MPI_COMM_WORLD);
            if (n == 0)
            {
                done = 1;
                continue;
            }
            MPI_Unpack(&packBuffer, sizePack, &packPosition, &xl, 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(&packBuffer, sizePack, &packPosition, &xh, 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(&packBuffer, sizePack, &packPosition, &p, 1, MPI_DOUBLE, MPI_COMM_WORLD);   
        }

        h = (xh - xl) / (double)n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += numprocs)
        {
            x = xl + h * ((double)i - 0.5);
            sum += f(x, p);
        }
        myfunk = h * sum;

        MPI_Reduce(&myfunk, &funk, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myid == 0)
        {
            endwtime = MPI_Wtime();
            printf("Integral is approximately  %.16f, Error   %.16f\n", funk, abs(funk - fi(xh, p) + fi(xl, p)));
            printf("Time of calculation = %f\n", endwtime - startwtime);
        }
        done = 1;

    }
    MPI_Finalize();
}

static double f(double a, double p)
{
    double c = 0.8;
    return sin(c * a) * cos(c * a);
}

static double fi(double a, double p)
{
    double c = 0.8;
    return (1 / (2 * c)) * pow(sin(c * a), 2);
}