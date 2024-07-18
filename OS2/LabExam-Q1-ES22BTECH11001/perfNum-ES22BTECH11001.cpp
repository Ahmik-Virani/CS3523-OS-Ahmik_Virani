/*****************************************
 * 
 * OS Exam question 1
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      April 30 2024
 * 
***********************************************/

#include<iostream>
#include<pthread.h>
#include<fstream>
#include<mutex>
#include<sys/time.h>

using namespace std;

mutex lockPrint;                //Lock to ensure there is no race condition while priniting in output file

int N, K;                       //Variables as mentioned in the question

atomic<int> counter(0);         //Atomic counter to count the total number of perfect numbers, and to ensure no race condition

//Code to calculate time of algorithm in micro seconds
long getTime()
{
    struct timeval start;
    gettimeofday(&start, NULL);
    return start.tv_sec * 1000000 + start.tv_usec;
}

//A structure that indicates the thread number and name of output file
struct arg_structure
{
    ofstream *file;
    int threadNumber;
};

//Function to check if a number is perfect or not
bool isPerfect(int x)
{
    int sum = 0;
    
    //Check all divisors from 1 to x - 1
    for(int i = 1 ; i < x ; i++)
    {
        if((x % i) == 0)
            sum += i;
    }

    //If sum equals x, increment counter
    if(sum == x)
    {
        counter.fetch_add(1);
        return true;
    }

    return false;
}

//Runner function, this is where the threads will run
void *runner(void *param)
{  
    //Take the input structure
    struct arg_structure *arguments = (struct arg_structure *)param;
    ofstream *outFile = arguments->file;
    int threadID = arguments->threadNumber;

    //K is the number of threads, so we are basically incrementing each value by K so no threads will compute the same number
    for(int i = threadID ; i <= N; i+=K)
    {
        if(isPerfect(i))
        {
            lockPrint.lock();                                                   //Ensure that no race condition by locking 
            *outFile << i << ": Found by thread " << threadID << endl;
            lockPrint.unlock();
        }
    }

    pthread_exit(0);
}

int main()
{
    ifstream inFile;
    inFile.open("inp.txt");

    inFile >> N >> K;
    inFile.close();

    pthread_t tid[K];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    int *threadNumber = new int [K];

    ofstream *outFile = new ofstream("OutFile.txt");

    /*start the timer*/
    long start = getTime();

    for(int i = 0 ; i < K ; i++)
    {
        struct arg_structure *args = new arg_structure;
        args->file = outFile;
        args->threadNumber = i + 1;         //Thread number starts from 1

        pthread_create(&tid[i], &attr, runner, (void *)args);
    }

    for(int i = 0 ; i < K ; i++)
    {
        pthread_join(tid[i], NULL);
    }

    long end = getTime();           //Ending the time

    *outFile << "Total Perfect Numbers: " << counter << endl;

    cout << "Exectution Time = " << end - start << " micro seconds";

    delete []threadNumber;

    return 0;
}