/*****************************************
 * 
 * Assignment 1, Method : Chunk
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      29/01/2024
 *
 * This question is about matrix multiplication using multithreading
 * I have solved this question using the method of chunks as described in the problem statement
 * 
***********************************************/

#include<iostream>
#include<fstream>
#include<pthread.h>
#include<cmath>
#include<algorithm>
#include<sys/time.h>

using namespace std;

//Code to calculate time of algorithm in micro seconds
long getTime()
{
    struct timeval start;
    gettimeofday(&start, NULL);
    return start.tv_sec * 1000000 + start.tv_usec;
}

int N, K;

/* Creating a 2D array */
int **A;  //Input
int **C;  //Output

//Compute the value of the chunkNo row
void multiply(int chunckNo)
{
    int ans;
    for(int i = 0 ; i < N ; i++)
    {
        ans = 0;
        for(int j = 0 ; j < N ; j++)
        {
            ans += A[chunckNo][j] * A[j][i];
        }

        C[chunckNo][i] = ans;
    }
}

//dividing the work into chuncks
void *runner(void *param)
{
    int p;
    if(N < K)
    {   
        p = 1;
    }

    else
    {
        p = N/K;
    }

    int k = (*(int *)param);

    //If the last thread is working, then all the remaing rows/chunks are alloted to it to ensure no chunk is left over
    if(k == K - 1)
    {
        for(int i = k * p ; i < N ; i++)
        {
            multiply(i);
        }
    }

    else
    {
        for(int i = k * p ; i < p * (k + 1) && i < N ; i++)
        {
            multiply(i);
        }
    }

    pthread_exit(0);
}

int main()
{

    /*start the timer*/
    long start = getTime();

    ifstream inFile;            //declaring input file variable
    inFile.open("inp.txt");     //Opening the input file

    inFile >> N >> K;

    //Dynamically allocate the input and output matrices before reading their value from the input file
    A = new int *[N];
    C = new int *[N];

    for(int i = 0 ; i <  N ; i++)
    {
        A[i] = new int[N];
    }

    for(int i = 0 ; i <  N ; i++)
    {
        C[i] = new int[N];
    }

    /* Taking input for the 2d array */
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            inFile >> A[i][j];
        }
    }

    inFile.close(); //Closing the file

    /* Main Code related to the threads part */

    pthread_t tid[K];           //Create K thread identifiers
    pthread_attr_t attr;        //set of thread attributes, will be same for all threads 

    pthread_attr_init(&attr);   //initializing attributes to default

    int *threadNumber = new int[K]; //An array to store the thread number

    //Creating threads, if we have more threads than size of matrix N, then no need to create K threads, only N are enough
    if(N > K)
    {
        for(int i = 0 ; i <  K ; i++)
        {
            threadNumber[i] = i;
            pthread_create(&tid[i], &attr, runner, &threadNumber[i]);
        }

        /* Wait for threads to finish */
        for(int i = 0 ; i < K ; i++)
        {
            pthread_join(tid[i], NULL);
        }
    
    }

    else
    {
        
        for(int i = 0 ; i <  N ; i++)
        {
            threadNumber[i] = i;
            pthread_create(&tid[i], &attr, runner, &threadNumber[i]);
        }

        /* Wait for threads to finish */
        for(int i = 0 ; i < N ; i++)
        {
            pthread_join(tid[i], NULL);
        }

    }

    ofstream outFile("out.txt");        //Declaring the output file variable

    outFile << "The square matrix is : " << endl << endl;

    //Displaying the output matrix in the output file
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            outFile << C[i][j] << " ";
        }

        outFile << endl;
        
    }

    long end = getTime();       //Stopping the timer

    outFile << endl << "Exectution Time = " << end - start << " micro seconds";

    outFile.close();            //Closing the output file after all necessary information is passed

    /* deallocating dynamically allocated memory in the 2d array */
    for(int i = 0 ; i < N ; i++)
    {
        delete []A[i];
    }
    delete []A;

    for(int i = 0 ; i < N ; i++)
    {
        delete []C[i];
    }
    delete []C;

    delete []threadNumber;

    return 0;
}