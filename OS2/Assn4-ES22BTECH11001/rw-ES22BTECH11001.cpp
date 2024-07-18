    /*****************************************
 * 
 * Assignment 4, Method : Writers Preference - readers writers program
 * Name:        Ahmik Virani
 * Roll Number: ES22BTECH11001
 * E-mail:      es22btech11001@iith.ac.in
 * Course:         Operating Systems - 2
 * Submitted:      17/03/2024
 *
 * This question is about solving the readers writers problem
 * I have solved this question using the Writers Preference - readers writers method explained in class
 * 
***********************************************/

#include <iostream>
#include <semaphore.h>
#include <fstream>
#include <pthread.h>
#include <ctime>
#include <unistd.h>
#include<random>
#include<string>
#include<ctime>
#include<sstream>
#include<cmath>
#include<mutex>

using namespace std;

//The following are the variables as defined in the problem statement
int nw, nr;
int kw, kr;
int muCS, muRem;

//The following variables are defined to calculate the required times that are asked in the questions for the experiment
double totalTimeWaiting = 0.0;
double waitingTimeForReaders = 0.0;
double waitingTimeForWriters = 0.0;
double worstCaseWaitingTimeReaders = 0.0;
double worstCaseWaitingTimeWriters = 0.0;
int NumberOfEntriesInCriticalSection = 0;
int NumberOfEntriesInCriticalSectionReader = 0;
int NumberOfEntriesInCriticalSectionWriter = 0;

//Two mutex locks are defined and will be used to avoid race conditions
mutex outputFileLock, lockForTime;

//The exponentially distributed function distributed about the given value of muCS or muRem in milli-seconds.
double exponentialRandom(double mean)
{
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> dist(1.0/mean);
    return dist(gen);
}

typedef struct _rwlock_t
{
    sem_t resource;         //Controls access (read/write) to the critical section
    sem_t rmutex;           //For syncing changes to shared variable readcount
    sem_t wmutex;           //For syncing changes to shared variable writeCount
    sem_t readTry;          //Semaphore to regulate access to the resource between readers and writers
    int readCount;          //Number of readers in critical section
    int writeCount;         //Number of writers in critical section
} rwlock_t;

//A structure created to pass the lock, output file and thread number from the pthread_create function to the corresponding reader/writer function
struct arg_structure
{
    rwlock_t *rw;
    ofstream *file;
    int threadNumber;
};

//Function to show the current system time in hr : min : sec : nanosecond
string getSysTime()
{
    time_t t = time(0);
    tm* now = localtime(&t);
    auto ns = chrono::system_clock::now();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(ns.time_since_epoch()) % 1000000000;

    
    ostringstream TIME;
    TIME << (now->tm_hour) << ":" << (now->tm_min) << ":" << (now->tm_sec) << ":" << nanoseconds.count();
    
    string s = TIME.str();

    return s;
}

//A function to initialize all the read write lock parameters
void rwlock_init(rwlock_t *rw)
{
    //Initialize the number of readers and writers in the critical section to 1
    rw->readCount = 0;
    rw->writeCount = 0;

    //Initialize all the binary semaphores to 1
    sem_init(&rw->resource, 0, 1);
    sem_init(&rw->rmutex, 0, 1);
    sem_init(&rw->readTry, 0, 1);
    sem_init(&rw->wmutex, 0, 1);
}

//Function for reader to acquire the lock
void rwlock_acquire_readlock (rwlock_t *rw)
{
    sem_wait(&rw->readTry);             //Indicate that reader is trying to enter
    sem_wait(&rw->rmutex);              //lock entry section to avoid race condition with other readers
    rw->readCount++;                    //report that a reader has enterer
    if(rw->readCount == 1)              //If the first reader
        sem_wait(&rw->resource);            //Lock the resource
    sem_post(&rw->rmutex);              //release entry section for other readers
    sem_post(&rw->readTry);             //indicate this reader is done trying to access the resource
}

//Function for reader to release the lock
void rwlock_release_readlock(rwlock_t *rw)
{
    sem_wait(&rw->rmutex);          //reserve exit section - avoids race condition with readers
    rw->readCount--;                //indicate the reader is leaving the critical section
    if(rw->readCount == 0)          //checks if this is the last reader leaving
        sem_post(&rw->resource);        //If so, release the locked resource
    sem_post(&rw->rmutex);          //release exit section for other readers
}

//Function for writer to acquire the lock
void rwlock_acquire_writelock(rwlock_t *rw)
{
    sem_wait(&rw->wmutex);          //reserve entry section for writers - avoids race conditions
    rw->writeCount++;               //report that a writer is entering
    if(rw->writeCount == 1)         //checks if this is the first writer
        sem_wait(&rw->readTry);         //If so, must lock the readers out. Prevent them from trying to enter critical section
    sem_post(&rw->wmutex);          //release entry section
    sem_wait(&rw->resource);        //reserve the resource for this writer - prevents other writers from simultaneously editing the shared resource
}

//Function for writer to release the lock
void rwlock_release_writelock(rwlock_t *rw)
{
    sem_wait(&rw->wmutex);          //reserve exit section
    rw->writeCount--;               //indicate writer is leaving
    if(rw->writeCount == 0)         //checks if this is the last writer
        sem_post(&rw->readTry);         //if so, must unlock the readers. Allows them to try enter critical section for reading
    sem_post(&rw->wmutex);          //release exit section
}

//Function for the writer thread, we pass the structure arg_structure as the input parameter
void writer(void *args)
{
    //We pass the structure containing the lock, output file and thread number to the thread
    struct arg_structure *arguments = (struct arg_structure *)args;
    rwlock_t *rw = arguments->rw;
    ofstream *outFile = arguments->file;
    int threadID = arguments->threadNumber;

    //pthread_t threadID = pthread_self();

    //delay values as defined in problem statement
    double randCSTime, randRemTime;

    //Each of the writer threads will access the shared object(or Critical Section) kw
    for (int i = 0; i < kw; i++)
    {
        string reqTime = getSysTime();
        
        outputFileLock.lock();          //We lock the writing process to the output file to ensure no race condition
        *outFile << i + 1 << "th CS request by Writer Thread " << threadID << " at " << reqTime << endl;
        outputFileLock.unlock();        //Unlock

        auto start = chrono :: high_resolution_clock::now();        //Start the timer just before the thread asks access for the critical section

        rwlock_acquire_writelock(rw);                               //Request entry to the critical section by requesting for the writeLock
        
        auto end = chrono :: high_resolution_clock::now();          //End time after entry to critical section granted
        double elapsedTime = chrono :: duration<double, milli>(end - start).count();

        lockForTime.lock();                     //We lock the various global variables to ensure no race conditions
        
        totalTimeWaiting += elapsedTime;
        waitingTimeForWriters += elapsedTime;
        NumberOfEntriesInCriticalSection++;
        NumberOfEntriesInCriticalSectionWriter++;
        worstCaseWaitingTimeWriters = max(worstCaseWaitingTimeWriters, elapsedTime);

        lockForTime.unlock();                   //Unlock

        string enterTime = getSysTime();
        
        //Locking file to prevent race condition as mentioned above
        outputFileLock.lock();
        *outFile << i + 1 << "th CS Entry by Writer Thread " << threadID << " at " << enterTime << endl;
        outputFileLock.unlock();
        
        randCSTime = exponentialRandom(muCS);
        sleep(randCSTime);                       //Simulate a thread writing in CS

        sem_post(&rw->resource);                //Releae the file

        rwlock_release_writelock(rw);           //Thread exit the critcal section

        string exitTime = getSysTime();

        //Locking file to prevent race condition as mentioned above
        outputFileLock.lock();
        *outFile << i + 1 << "th CS Exit by Writer Thread " << threadID << " at " << exitTime << endl;
        outputFileLock.unlock();

        randRemTime = exponentialRandom(muRem);
        sleep(randRemTime); //Simulate a thread executing in Remainder Section
    }
}

//Function for the reader thread, we pass the structure arg_structure as the input parameter
void reader(void *args)
{
    //We pass the structure containing the lock, output file and thread number to the thread
    struct arg_structure *arguments = (struct arg_structure *)args;
    rwlock_t *rw = arguments->rw;
    ofstream *outFile = arguments->file;
    int threadID = arguments->threadNumber;

    //pthread_t threadID = pthread_self();
    
    //delay values as defined in problem statement
    double randCSTime, randRemTime;

    //Each of the reader threads will access the shared object(or Critical Section) kr
    for (int i = 0; i < kr; i++)
    {
        string reqTime = getSysTime();

        outputFileLock.lock();          //We lock the writing process to the output file to ensure no race condition
        *outFile << i + 1 << "th CS request by Reader Thread " << threadID << " at " << reqTime << endl;
        outputFileLock.unlock();        //Unlock

        auto start = chrono :: high_resolution_clock::now();        //Start the timer just before the thread asks access for the critical section

        rwlock_acquire_readlock(rw);                                //Request entry to the critical section by requesting for the readlock
        
        auto end = chrono :: high_resolution_clock::now();          //End timer after entry to critical section granted
        double elapsedTime = chrono :: duration<double, milli>(end - start).count();

        lockForTime.lock();                 //We lock the various global variables to ensure no race conditions
        
        totalTimeWaiting += elapsedTime; 
        waitingTimeForReaders += elapsedTime;
        NumberOfEntriesInCriticalSectionReader++;
        NumberOfEntriesInCriticalSection++;
        worstCaseWaitingTimeReaders = max(worstCaseWaitingTimeReaders, elapsedTime);

        lockForTime.unlock();               //Unlock

        string enterTime = getSysTime();

        //Locking file to prevent race condition as mentioned above
        outputFileLock.lock();
        *outFile << i + 1 << "th CS Entry by Reader Thread " << threadID << " at " << enterTime << endl;
        outputFileLock.unlock();
        
        randCSTime = exponentialRandom(muCS);
        sleep(randCSTime);              //Simulate a thread reading from CS

        rwlock_release_readlock(rw);    //Thread exit the critcal section

        string exitTime = getSysTime();
        
        //Locking file to prevent race condition as mentioned above
        outputFileLock.lock();
        *outFile << i + 1 << "th CS Exit by Reader Thread " << threadID << " at " << exitTime << endl;
        outputFileLock.unlock();
        
        randCSTime = exponentialRandom(muRem);
        sleep(randRemTime);     //Simulate a thread executing in Remainder Section
    }
}

int main()
{
    ifstream inFile;            //declaring input file variable
    inFile.open("inp-params.txt");     //Opening the input file

    //Taking input from the input file
    inFile >> nw >> nr >> kw >> kr >> muCS >> muRem;

    inFile.close();     //Close the input file

    //Convert muCS and muRem into milliseconds
    muCS /= pow(10, 3);
    muRem /= pow(10, 3);

    pthread_t writersThreads[nw];           //Create nw writer thread identifiers
    pthread_t readersThreads[nr];           //Create nr reader thread identifiers
    pthread_attr_t attr;                    //set of thread attributes, will be same for all threads 

    pthread_attr_init(&attr);   //initializing attributes to default

    rwlock_t rwlock;        //Creating the readers writers lock
    rwlock_init(&rwlock);   //Initializing the lock variables

    //Open the output file
    ofstream *outFile = new ofstream("RW-log.txt");


    //Create writer threads
    for(int i = 0 ; i < nw ; i++)
    {
        struct arg_structure *args = new arg_structure;
        args->file = outFile;
        args->rw = &rwlock;
        args->threadNumber = i + 1;         //Thread number starts from 1

        pthread_create(&writersThreads[i], &attr, (void*(*)(void*))writer, (void *)args);       //Pass the arguments sturcture to the thread
    }

    //Create reader threads
    for(int i = 0 ; i < nr ; i++)
    {
        struct arg_structure *args = new arg_structure;
        args->file = outFile;
        args->rw = &rwlock;
        args->threadNumber = i + 1;         //Thread number starts from 1

        pthread_create(&readersThreads[i], &attr, (void*(*)(void*))reader, (void *)args);       //Pass the arguments sturcture to the thread
    }

    //Wait for writer threads to complete
    for(int i = 0 ; i < nw ; i++)
    {
        pthread_join(writersThreads[i], NULL);
    }

    //Wait for reader threads to complete
    for(int i = 0 ; i < nr ; i++)
    {
        pthread_join(readersThreads[i], NULL);
    }

    //Free dynamically allocated memory
    delete outFile;

    //Finally creating Average_time.txt file
    ofstream avgTimeFile;

    avgTimeFile.open("Average_time.txt");

    avgTimeFile << "Average waiting time for all threads in writers preference solution is " << totalTimeWaiting/NumberOfEntriesInCriticalSection << " milliseconds"<< endl;
    avgTimeFile << "Average waiting time for writers threads in writers preference solution is " << waitingTimeForWriters/NumberOfEntriesInCriticalSectionWriter << " milliseconds" << endl;
    avgTimeFile << "Average waiting time for readers threads in writers preference solution is " << waitingTimeForReaders/NumberOfEntriesInCriticalSectionReader << " milliseconds" << endl;
    cout << "Worst case waiting time for writers threads in writers preference solution is " << worstCaseWaitingTimeWriters << " milliseconds" << endl;
    cout << "Worst case waiting time for readers threads in writers preference solution is " << worstCaseWaitingTimeReaders << " milliseconds" << endl;

    avgTimeFile.close();

    return 0;
}