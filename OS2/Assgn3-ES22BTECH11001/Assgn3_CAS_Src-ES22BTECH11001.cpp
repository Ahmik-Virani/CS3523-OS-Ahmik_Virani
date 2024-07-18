/*****************************************
 * 
 * Assignment 3, Method : Compare And Swap (CAS)
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      04/02/2024
 *
 * This question is about Dynamic Matrix Squaring
 * I have solved this question using the method of Compare and Swap
 * 
***********************************************/

#include<iostream>
#include<fstream>
#include<pthread.h>
#include<cmath>
#include<sys/time.h>
#include<atomic>
#include<thread>
#include<vector>

using namespace std;

atomic<bool> cnt (0);
mutex n_mutex;

//Code to calculate time of algorithm in micro seconds
long getTime()
{
    struct timeval start;
    gettimeofday(&start, NULL);
    return start.tv_sec * 1000000 + start.tv_usec;
}

//Global variable with the same variable names as given in the problem statement
int N, K, rowInc;       //These will be input from the input file
int sharedCounter = 0;  //This will indicate the current row being processeed

/* Creating a 2D array */
int **A;  //Input
int **C;  //Output

//Compute the value of the rowNo row
void multiply(int rowNo)
{
    int ans;
    for(int i = 0 ; i < N ; i++)
    {
        ans = 0;
        for(int j = 0 ; j < N ; j++)
        {
            ans += A[rowNo][j] * A[j][i];
        }

        C[rowNo][i] = ans;
    }
}

//dividing the work to threads, the input parameter shows the thread number
void *runner(void *param)
{
    int k = (*(int *)param);            //Using the input parameter as the thread number and storing it in the integer data type

    bool old_value = false;


    //A loop that runs till all the rows of the output matrix has been calculated
    do
    {
        int i;

        //Acquire the lock
        while(!cnt.compare_exchange_strong(old_value, true)) {
            old_value = false;
        };
        
        //Critical Section
        {
            i = sharedCounter;
            sharedCounter += rowInc;
        }

        //Free Critical Section
        cnt = false;

        //Remainder Section
        for(int j = i ; j < rowInc + i && j < N ; j++)
        {
            multiply(j);
        }
        
    }while(sharedCounter < N);

    pthread_exit(0);
}

int main()
{

    /*start the timer*/
    long start = getTime();

    ifstream inFile;            //declaring input file variable
    inFile.open("inp.txt");     //Opening the input file

    inFile >> N >> K >> rowInc; //Taking in input from the input file
    sharedCounter = 0;          //Initialize the shared counter to zero

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

    int *threadNumber = new int[K]; //An array to store the thread number
    
    // Here we are creating a vector with data type thread, which implicitely handles the attributes
    vector<thread> threads;
    
    for(int i = 0 ; i < K ; i++)
    {
        threadNumber[i] = i;                                                //Each thread is identified by a unique thread number
        threads.push_back(thread(runner, &threadNumber[i]));                //We pass each thread to the runner function and also pass the thread's unique id (thread number)
    }

    //We wait for all threads to finish
    for(auto &th : threads)
    {
        th.join();
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