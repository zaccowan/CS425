Problem 1: Hello World with MPI
<br>
Prompt: Write a simple MPI program that initializes the MPI environment, prints "Hello World" from 16 processes, and then finalizes the MPI environment.

*Answer for Problem 1*
N/A - see code in repo and command line arguments to run code.
<br>

``` cmd
mpicc hello.c -o hello
mpirun -np 16 -f ../machinefile ./hello
```
<br>

*Sample Result for Problem 1*

![Hello world result from code testing](/hello-ex.png "Hello world program running on cluster.")

<br>
Problem  2: Sum of Array Elements
<br>
Prompt: Write an MPI program that distributes an array of the first 1000000, 1 through  1000000 integers among 4 processes and calculates the sum of the array elements using MPI. 
Each process should compute the sum of its portion of the array and then use MPI to gather the results and compute the total sum. Capture the wall time.
Repeat with 30 processes and compare the wall time. Be sure and have your code confirm the correctness of the calculation using Gauss’s formula: S = n(n+1)/2 where S is the sum and n is the value of the last integer in the sequence.

*Answer for Problem 2*
<br>

``` cmd
mpicc arrarySum.c -o arraySum
mpirun -np 4 -f ../machinefile ./arraySum
mpirun -np 30 -f ../machinefile ./arraySum
```

<br>

*Sample Result for Problem 2*

![Array Sum result from code testing](/array-ex.png "Array sum program running on cluster.")

<br>
Problem 3: Matrix Multiplication
<br>
Prompt: Write an MPI program to perform matrix multiplication with a 10000 x 10000 matrix of random numbers. The program should distribute the rows of the first matrix among the processes and compute the resulting matrix in parallel. Calculate the result with serial code to confirm your results. How much faster was the parallel program?

*Answer for Problem 3*
Aprox speedup: 10-12 times faster for parallel code factoring in slight variations in computation time.
<br>

``` cmd
mpicc matrix.c -o matrix
mpirun -np 25 -f ../machinefile ./matrix
```

<br>

*Sample Result for Problem 3*

![Matrix result from code testing](/matrix-2500.png "Matrix program running on cluster.")
