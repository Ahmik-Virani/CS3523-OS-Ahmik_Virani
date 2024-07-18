/*****************************************
 * 
 * Assignment 2, Method : Chunk
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      23/02/2024
 *
 * This question is about matrix multiplication using thread affinity
 * I have solved this question using the method of chunks as described in the problem statement
 * 
***********************************************/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
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

//These are the global variables, with the same notation as specified in the prblem statement
int N, K, C, BT;

/* Creating a 2D array */
int **A;  //Input
int **OUT;  //Output

//These are addition global arrays
int *timeBounded;		//This will keep track of times taken by all the bounded threads
int *timeUnbounded;	//This will keep track of times taken by all the unbounded threads

//Compute the values at the 'rowNo' row of the output matrix
void multiply(int chunckNo)
{
    int ans;
    for(int i = 0 ; i < N ; i++)
    {
        ans = 0;				//For each element in output matrix (of specified rowNo in OUT), we initialize the ans variable, i.e. it's value to zero
        for(int j = 0 ; j < N ; j++)
        {	
            ans += A[chunckNo][j] * A[j][i];	//Computing the value of the required row elements
        }

        OUT[chunckNo][i] = ans;			//Storing the value in the corresponding row and column in output matrix (OUT)
    }	
}

//dividing the work into chuncks
void *runner(void *param)
{
    long start = getTime();		//start the timer for this particular thread

    int k = (*(int *)param);		//Storing the input parameter as an integer value in variable k
    
//    int b = K/C;			//b = K/C as specified in the problem statement
    int b = 0;				//b = 0, to compute values corresponding to assignment 1

    //Assign the CPU for the bounded threads
    if(k < BT && b != 0)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);

        int s;

	//set affinity to include the cpu k/b
        CPU_SET(k/b , &cpuset);				//The value k/b works because, the first b threads (with thread numbers 0->b-1, its value of k/b will be 0, next b will be 1 and so on)

        s = pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);

	//Error message
        if(s != 0)
        {
            perror("pthread_getaffinity_np");
        }
    }

    int p;
    if(N < K)
    {   
        p = 1;
    }

    else
    {
        p = N/K;
    }

     //Compute the output matrix values of row number (i), as mentioned according to mixed matrix multiplication definition
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

    long end = getTime();               //Stopping the timer

    //Add the time according to weather the thread was bounded or not   
    if(k < BT && b != 0)
    	timeBounded[k] = end - start;
    else
    	timeUnbounded[k - BT] = end - start;

    //exit the thread
    pthread_exit(0);
}

int main()
{

    ifstream inFile;            //declaring input file variable
    inFile.open("inp.txt");     //Opening the input file

    inFile >> N >> K >> C >> BT;

    //Dynamically allocate the input and output matrices before reading their value from the input file
    A = new int *[N];
    OUT = new int *[N];

    for(int i = 0 ; i <  N ; i++)
    {
        A[i] = new int[N];
    }

    for(int i = 0 ; i <  N ; i++)
    {
        OUT[i] = new int[N];
    }

    /* Taking input for the 2d array */
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            inFile >> A[i][j];
        }
    }

    //Dynamically allocating the time related global arrays
    timeBounded = new int [BT];
    timeUnbounded = new int [K - BT];

    inFile.close(); //Closing the file

    /* Main Code related to the threads part */

    pthread_t tid[K];           //Create K thread identifiers
    pthread_attr_t attr;        //set of thread attributes, will be same for all threads 

    pthread_attr_init(&attr);   //initializing attributes to default

    int *threadNumber = new int[K]; //An array to store the thread number

    /*start the timer*/
    long start = getTime();

    //Creating threads, if we have more threads than size of matrix N, then no need to create K threads, only N are enough
    if(N > K)
    {
        for(int i = 0 ; i <  K ; i++)
        {

            threadNumber[i] = i;
            pthread_create(&tid[i], &attr, runner, &threadNumber[i]);		//Passing the thread number as the input parameter to the runner function
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
            pthread_create(&tid[i], &attr, runner, &threadNumber[i]);		//Passing the thread number as the input parameter to the runner function

        }

        /* Wait for threads to finish */
        for(int i = 0 ; i < N ; i++)
        {
            pthread_join(tid[i], NULL);
        }

    }
    
    long end = getTime();       //Stopping the timer

    ofstream outFile("out.txt");        //Declaring the output file variable

    outFile << "The square matrix is : " << endl << endl;

    //Displaying the output matrix in the output file
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            outFile << OUT[i][j] << " ";
        }

        outFile << endl;
        
    }

    outFile << endl << "Exectution Time = " << end - start << " micro seconds";

    outFile.close();            //Closing the output file after all necessary information is passed

    int sumBoundedTime = 0;
    int sumUnboundedTime = 0;

    //int b = K/C;
    int b = 0;	

    if (b != 0)
    {
    	for(int i = 0 ; i < BT ; i++)
    	{
    	    sumBoundedTime += timeBounded[i];
    	}
    }

    for(int i = 0 ; i < (K - BT) ; i++)
    {
        sumUnboundedTime += timeUnbounded[i];
    }
    
    cout << "The average time taken for bounded threads is : " << sumBoundedTime / BT << endl;
    cout << "The average time taken for unbounded threads is : " << sumUnboundedTime / (K - BT) << endl;

    /* deallocating dynamically allocated memory in the 2d array */
    for(int i = 0 ; i < N ; i++)
    {
        delete []A[i];
    }
    delete []A;

    delete []threadNumber;

    return 0;
}
