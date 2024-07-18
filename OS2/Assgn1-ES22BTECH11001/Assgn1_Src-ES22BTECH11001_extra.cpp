/*****************************************
 * 
 * Assignment 1, Method : extra credit
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      29/01/2024
 *
 * This question is about matrix multiplication using multithreading
 * I have solved this question using the my own method
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


//Compute the value of the resultant matrix at the index (rowNo, colNo)
void multiply(int rowNo, int colNo)
{
    int ans = 0;
    for(int i = 0 ; i < N ; i++)
    {
        ans += A[rowNo][i] * A[i][colNo];
    }


    C[rowNo][colNo] = ans;
}

//dividing the work as per each value in the output matrix
void *runner(void *param)
{
    int dummyRow = 0;


    int k = (*(int *)param);

    for(int i = k; dummyRow < N ; i += K)
    {
        if(i >= N)
        {
            i = i % N;
            dummyRow++;
        }

        if(dummyRow == N)
            break;

        multiply(dummyRow, i);
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

    /* Main Code related to threading*/

    pthread_t tid[K];       //Create K thread identifiers
    pthread_attr_t attr;    //set of thread attributes, will be same for all threads 

    pthread_attr_init(&attr);   //initializing attributes to default

    int *threadNumber = new int[K]; //An array to store the thread number

    /* Create K threads*/
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
    

    ofstream outFile("out.txt");                            //Declaring the output variable

    outFile << "The square matrix is : " << endl << endl;   

    /*Printing the answer in the output file*/
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {
            outFile << C[i][j] << " ";
        }

        outFile << endl;
        
    }

    long end = getTime();           //Ending the time

    outFile << endl << "Exectution Time = " << end - start << " micro seconds";

    outFile.close();                //Closing the output file

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