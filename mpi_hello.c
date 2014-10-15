#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * This is a quick Hello World example for MPI.
 */

int main(int argc, char *argv[])
{
    //Initialize, very important!
    MPI_Init(&argc, &argv);

    //Determine my number, and the total number of processes.
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //Make a number, and tell the world who you are!
    srand(rank);
    rand();
    double x = ((double) rand()) / ((double) RAND_MAX);
    printf("Hello from %d of %d, my number is %lf.\n", rank, size, x);

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        printf("\n");

    //Talk to your neighbours, and print their numbers.
    double xL, xR;
    int left, right;

    //Neighbour to the left.
    left = rank-1;
    if(left == -1)
        left = size-1;
    //Neighbour to the right.
    right = (rank+1)%size;

    //Communicate!
    MPI_Sendrecv(&x, 1, MPI_DOUBLE, left, 0,
                &xR, 1, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, NULL);
    MPI_Sendrecv(&x, 1, MPI_DOUBLE, right, 0,
                &xL, 1, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, NULL);
    printf("This is %d, left is %lf, right is %lf.\n", rank, xL, xR);
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        printf("\n");

    //Calculate the sum of all the numbers, and tell all the processes!
    double max, sum;
    MPI_Allreduce(&x, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    printf("The sum is %lf\n", sum);

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        printf("\n");

    //Find the biggest number, but tell only the root (0) process.
    MPI_Reduce(   &x, &max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(rank == 0)
        printf("The Biggest Number is %lf.\n\n", max);

    //Clean up. Even more important!
    MPI_Finalize();

    return 0;
}
